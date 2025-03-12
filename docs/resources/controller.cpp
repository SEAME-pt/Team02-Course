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
        cudaStreamCreate(&stream);

        // Allocate GPU memory
        cudaMalloc(&inputDevice, 3 * 256 * 512 * sizeof(float));
        cudaMalloc(&outputDevice, 2 * 256 * 512 * sizeof(float));

        inputData.resize(3 * 256 * 512);
        outputData.resize(2 * 256 * 512);
    }

    ~LaneDetector()
    {
        cudaFree(inputDevice);
        cudaFree(outputDevice);
        cudaStreamDestroy(stream);
    }

    void detect(cv::Mat& frame)
    {
        // Preprocess
        preProcess(frame);

        // Copy to GPU
        cudaMemcpyAsync(inputDevice, inputData.data(),
                        3 * 256 * 512 * sizeof(float), cudaMemcpyHostToDevice,
                        stream);

        // Run inference
        void* bindings[] = {inputDevice, outputDevice};
        context->enqueueV2(bindings, stream, nullptr);

        // Copy back to CPU
        cudaMemcpyAsync(outputData.data(), outputDevice,
                        2 * 256 * 512 * sizeof(float), cudaMemcpyDeviceToHost,
                        stream);

        cudaStreamSynchronize(stream);

        // Postprocess
        postProcess(frame);
    }

    void run(const std::string& pipeline)
    {
        cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
        if (!cap.isOpened())
        {
            throw std::runtime_error("Failed to open camera");
        }

        cv::Mat frame;
        while (true)
        {
            if (!cap.read(frame))
            {
                break;
            }

            detect(frame);

            cv::imshow("Lane Detection", frame);

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
    std::vector<float> inputData;
    std::vector<float> outputData;

    void preProcess(const cv::Mat& frame)
    {
        static cv::Mat resized(256, 512, CV_8UC3); // Reuse Mat objects
        static cv::Mat float_mat(256, 512, CV_32FC3);

        // Use INTER_LINEAR for better performance vs quality trade-off
        cv::resize(frame, resized, cv::Size(512, 256), 0, 0, cv::INTER_LINEAR);
        resized.convertTo(float_mat, CV_32F, 1.0f / 255.0f);

        // Use pointer arithmetic for faster memory access
        float* input_data    = inputData.data();
        float* mat_data      = (float*)float_mat.data;
        const int plane_size = 256 * 512;

        // Optimize channel conversion using memcpy
        for (int c = 0; c < 3; c++)
        {
            for (int i = 0; i < plane_size; i++)
            {
                input_data[c * plane_size + i] = mat_data[i * 3 + c];
            }
        }
    }

    void postProcess(cv::Mat& frame)
    {
        static cv::Mat mask(256, 512, CV_8UC1); // Reuse Mat objects
        static cv::Mat resized_mask;
        static cv::Mat colored_mask;

        // Use pointer arithmetic for faster access
        uchar* mask_data       = mask.data;
        float* output_data     = outputData.data();
        const int total_pixels = 256 * 512;

#pragma omp parallel for // Enable OpenMP for parallelization
        for (int i = 0; i < total_pixels; i++)
        {
            float p0     = output_data[i];
            float p1     = output_data[total_pixels + i];
            mask_data[i] = (p1 > p0) ? 255 : 0;
        }

        cv::resize(mask, resized_mask, frame.size(), 0, 0, cv::INTER_LINEAR);
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
    const std::string enginePath = "lane_segmentation.engine";
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
