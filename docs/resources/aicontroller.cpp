#include "opencv2/opencv.hpp"
#include "cuda.h"
#include "NvInfer.h"
#include "NvOnnxParser.h"
#include <fstream>
#include <iostream>
#include <memory>

#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <condition_variable>

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
        stopThreads = true;
        condition.notify_all();
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
        stopThreads = false;
        std::thread capture_thread(&LaneDetector::captureThread, this,
                                   pipeline);
        std::thread process_thread(&LaneDetector::processThread, this);

        capture_thread.join();
        process_thread.join();
    }

  private:
    std::shared_ptr<nvinfer1::IExecutionContext> context;
    cudaStream_t stream;
    void* inputDevice;
    void* outputDevice;
    std::vector<float> inputData;
    std::vector<float> outputData;

    // Threading members
    std::queue<cv::Mat> frameQueue;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stopThreads{false};
    static const int MAX_QUEUE_SIZE = 4;

    void captureThread(const std::string& pipeline)
    {
        cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
        if (!cap.isOpened())
        {
            throw std::runtime_error("Failed to open camera");
        }

        while (!stopThreads)
        {
            cv::Mat frame;
            if (!cap.read(frame))
                continue;

            std::lock_guard<std::mutex> lock(queueMutex);
            if (frameQueue.size() < MAX_QUEUE_SIZE)
            {
                frameQueue.push(frame.clone());
                condition.notify_one();
            }
        }
        cap.release();
    }

    void processThread()
    {
        while (!stopThreads)
        {
            cv::Mat frame;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                condition.wait(lock, [this]
                               { return !frameQueue.empty() || stopThreads; });

                if (stopThreads && frameQueue.empty())
                    break;

                frame = frameQueue.front();
                frameQueue.pop();
            }

            detect(frame);

            cv::imshow("Lane Detection", frame);
            if (cv::waitKey(1) & 0xFF == 'q')
            {
                stopThreads = true;
            }
        }
    }

    void preProcess(const cv::Mat& frame)
    {
        cv::Mat resized, float_mat;
        cv::resize(frame, resized, cv::Size(512, 256));
        resized.convertTo(float_mat, CV_32F, 1.0 / 255.0);

        // Convert to NCHW format
        float* input_data = inputData.data();
        for (int c = 0; c < 3; c++)
        {
            for (int h = 0; h < 256; h++)
            {
                for (int w = 0; w < 512; w++)
                {
                    input_data[c * 256 * 512 + h * 512 + w] =
                        float_mat.at<cv::Vec3f>(h, w)[c];
                }
            }
        }
    }

    void postProcess(cv::Mat& frame)
    {
        cv::Mat mask(256, 512, CV_8UC1);
        for (int h = 0; h < 256; h++)
        {
            for (int w = 0; w < 512; w++)
            {
                float p0             = outputData[h * 512 + w];
                float p1             = outputData[256 * 512 + h * 512 + w];
                mask.at<uchar>(h, w) = (p1 > p0) ? 255 : 0;
            }
        }

        cv::Mat resized_mask;
        cv::resize(mask, resized_mask, frame.size());

        // Overlay the mask on the frame
        cv::Mat colored_mask;
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