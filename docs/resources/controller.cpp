#include "opencv2/opencv.hpp"
#include "cuda.h"
#include "NvInfer.h"
#include "NvOnnxParser.h"
#include <omp.h>
#include <fstream>
#include <iostream>
#include <memory>

class Logger : public nvinfer1::ILogger
{
    void log(Severity severity, const char* msg) noexcept override
    {
        if (severity <= Severity::kWARNING)
        {
            std::cout << msg << std::endl;
        }
    }
} logger;

class LaneDetector
{
  public:
    LaneDetector(const std::string& enginePath)
    {
        createExecutionContext(enginePath);

        // Set highest stream priority
        int leastPriority, greatestPriority;
        cudaDeviceGetStreamPriorityRange(&leastPriority, &greatestPriority);
        cudaStreamCreateWithPriority(&stream, cudaStreamNonBlocking,
                                     greatestPriority);

        // Pin memory for faster transfers
        void* input_ptr;
        void* output_ptr;
        cudaHostAlloc(&input_ptr, 3 * 128 * 256 * sizeof(float),
                      cudaHostAllocMapped);
        cudaHostAlloc(&output_ptr, 2 * 128 * 256 * sizeof(float),
                      cudaHostAllocMapped);
        inputData  = static_cast<float*>(input_ptr);
        outputData = static_cast<float*>(output_ptr);

        // Allocate GPU memory
        size_t pitch;
        cudaMallocPitch(&inputDevice, &pitch, 256 * sizeof(float), 128 * 3);
        cudaMallocPitch(&outputDevice, &pitch, 256 * sizeof(float), 128 * 2);
    }

    ~LaneDetector()
    {
        cudaFreeHost(inputData);
        cudaFreeHost(outputData);
        cudaFree(inputDevice);
        cudaFree(outputDevice);
        cudaStreamDestroy(stream);
    }

    void detect(cv::Mat& frame)
    {
        static cudaEvent_t start, stop; // Make events static
        static bool eventsCreated = false;
        static cudaGraph_t graph;
        static cudaGraphExec_t graphExec;
        static bool graphCreated = false;

        if (!eventsCreated)
        {
            cudaEventCreate(&start);
            cudaEventCreate(&stop);
            eventsCreated = true;
        }

        cudaEventRecord(start, stream);

        // Preprocess
        preProcess(frame);

        // Copy to GPU
        cudaMemcpyAsync(inputDevice, inputData, 3 * 128 * 256 * sizeof(float),
                        cudaMemcpyHostToDevice, stream);

        // Run inference with optimization flags
        void* bindings[] = {inputDevice, outputDevice};
        context->enqueueV2(bindings, stream, nullptr);

        // Copy back to CPU
        cudaMemcpyAsync(outputData, outputDevice, 2 * 128 * 256 * sizeof(float),
                        cudaMemcpyDeviceToHost, stream);

        cudaStreamSynchronize(stream);

        // Postprocess
        postProcess(frame);

        cudaEventRecord(stop, stream);
        cudaEventSynchronize(stop);

        float milliseconds = 0;
        cudaEventElapsedTime(&milliseconds, start, stop);
        std::cout << "Inference time: " << milliseconds << "ms\n";
    }

    void run(const std::string& pipeline)
    {
        cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
        if (!cap.isOpened())
        {
            throw std::runtime_error("Failed to open camera");
        }

        cv::Mat frame;
        int frame_count      = 0;
        const int FRAME_SKIP = 8; // Process every other frame

        // Set camera buffer size
        cap.set(cv::CAP_PROP_BUFFERSIZE, 1);

        while (true)
        {
            if (!cap.read(frame))
            {
                break;
            }

            if (frame_count % FRAME_SKIP == 0)
            {
                detect(frame);
                cv::imshow("Lane Detection", frame);
            }
            frame_count++;

            if (cv::waitKey(1) == 'q')
                break;
        }

        cv::destroyAllWindows();
    }

  private:
    std::shared_ptr<nvinfer1::IExecutionContext> context;
    cudaStream_t stream;
    void* inputDevice;
    void* outputDevice;
    float* inputData;
    float* outputData;

    void preProcess(const cv::Mat& frame)
    {
        static cv::Mat resized(128, 256, CV_8UC3);
        static cv::Mat float_mat(128, 256, CV_32FC3);

        // Use INTER_NEAREST for faster resizing
        cv::resize(frame, resized, cv::Size(256, 128), 0, 0, cv::INTER_NEAREST);

        // Optimize memory access pattern
        const int plane_size      = 128 * 256;
        const uint8_t* frame_data = resized.data;

        // Set number of threads for OpenMP
        omp_set_num_threads(4);

// Use collapse(2) to better parallelize nested loops
#pragma omp parallel for collapse(2) schedule(static)
        for (int c = 0; c < 3; c++)
        {
            for (int i = 0; i < plane_size; i++)
            {
                // Direct memory access optimization
                inputData[c * plane_size + i] = frame_data[i * 3 + c] / 255.0f;
            }
        }
    }

    void postProcess(cv::Mat& frame)
    {
        static cv::Mat mask(128, 256, CV_8UC1);
        static cv::Mat resized_mask;
        static cv::Mat colored_mask;

        uchar* mask_data       = mask.data;
        const int total_pixels = 128 * 256;

#pragma omp parallel for
        for (int i = 0; i < total_pixels; i++)
        {
            float p0     = outputData[i];
            float p1     = outputData[total_pixels + i];
            mask_data[i] = (p1 > p0) ? 255 : 0;
        }

        cv::resize(mask, resized_mask, frame.size(), 0, 0, cv::INTER_NEAREST);
        cv::cvtColor(resized_mask, colored_mask, cv::COLOR_GRAY2BGR);
        cv::addWeighted(frame, 1.0, colored_mask, 0.5, 0.0, frame);
    }

    void createExecutionContext(const std::string& enginePath)
    {
        std::ifstream file(enginePath, std::ios::binary);
        if (!file)
        {
            throw std::runtime_error("Failed to open engine file");
        }

        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> engineData(size);
        file.read(engineData.data(), size);

        nvinfer1::IRuntime* runtime = nvinfer1::createInferRuntime(logger);
        nvinfer1::ICudaEngine* engine =
            runtime->deserializeCudaEngine(engineData.data(), size);
        context.reset(engine->createExecutionContext());
    }
};

int main(int argc, char** argv)
{
    const std::string enginePath = "model_segmentation-128-256.engine";
    const std::string pipeline =
        "nvarguscamerasrc sensor-id=0 ! "
        "video/x-raw(memory:NVMM), width=640, height=480, format=NV12, "
        "framerate=30/1 ! "
        "nvvidconv ! video/x-raw, format=BGRx ! "
        "videoconvert ! video/x-raw, format=BGR ! "
        "appsink";

    try
    {
        LaneDetector detector(enginePath);
        detector.run(pipeline);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
