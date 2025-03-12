#include "opencv2/opencv.hpp"
#include "cuda.h"
#include "NvInfer.h"
#include "NvOnnxParser.h"
#include <fstream>
#include <iostream>
#include <memory>

class Logger : public nvinfer1::ILogger
{
  public:
    void log(Severity severity, const char* msg) noexcept override
    {
        if (severity <= Severity::kWARNING)
            std::cout << msg << std::endl;
    }
};

// Smart pointer for automatic resource management
struct TRTDestroy
{
    template <class T> void operator()(T* obj) const
    {
        if (obj)
            delete obj;
    }
};

template <class T> using TRTUniquePtr = std::unique_ptr<T, TRTDestroy>;

int main(void)
{
    Logger logger;
    int memorySize               = 1024;
    const std::string onnxFile   = "lane_segmentation.onnx";
    const std::string enginePath = "lane_segmentation.engine";

    // Check if the ONNX file exists
    std::ifstream ifile(onnxFile);
    if (!ifile)
    {
        std::cerr << "Error: Could not open file " << onnxFile << std::endl;
        return 1;
    }

    // Create builder and network with smart pointers
    TRTUniquePtr<nvinfer1::IBuilder> builder(
        nvinfer1::createInferBuilder(logger));
    const auto explicitBatch =
        1U << static_cast<uint32_t>(
            nvinfer1::NetworkDefinitionCreationFlag::kEXPLICIT_BATCH);
    TRTUniquePtr<nvinfer1::INetworkDefinition> network(
        builder->createNetworkV2(explicitBatch));

    // Create ONNX parser
    TRTUniquePtr<nvonnxparser::IParser> parser(
        nvonnxparser::createParser(*network, logger));

    // Parse ONNX file
    if (!parser->parseFromFile(onnxFile.c_str(), 2))
    {
        std::cerr << "Error: Failed to parse ONNX model from file: " << onnxFile
                  << std::endl;
        for (int i = 0; i < parser->getNbErrors(); ++i)
        {
            std::cerr << "Parser error: " << parser->getError(i)->desc()
                      << std::endl;
        }
        return 1;
    }
    std::cout << "TensorRT loaded ONNX model successfully." << std::endl;

    // Create config with smart pointer
    TRTUniquePtr<nvinfer1::IBuilderConfig> config(
        builder->createBuilderConfig());
    config->setMaxWorkspaceSize(1024 * 1024 * memorySize);
    config->setFlag(nvinfer1::BuilderFlag::kFP16);

    nvinfer1::IOptimizationProfile* profile =
        builder->createOptimizationProfile();

    // Set dimensions matching your ONNX model:
    // Batch size: 1
    // Channels: 3 (RGB)
    // Height: 256
    // Width: 512
    profile->setDimensions("input", // Input tensor name from ONNX model
                           nvinfer1::OptProfileSelector::kMIN,
                           nvinfer1::Dims4(1, 3, 256, 512));
    profile->setDimensions("input", nvinfer1::OptProfileSelector::kOPT,
                           nvinfer1::Dims4(1, 3, 256, 512));
    profile->setDimensions("input", nvinfer1::OptProfileSelector::kMAX,
                           nvinfer1::Dims4(1, 3, 256, 512));

    config->addOptimizationProfile(profile);

    // Build engine
    std::cout << "Building TensorRT engine..." << std::endl;
    TRTUniquePtr<nvinfer1::IHostMemory> serializedEngine(
        builder->buildSerializedNetwork(*network, *config));
    if (!serializedEngine)
    {
        std::cerr << "Failed to build TensorRT engine" << std::endl;
        return 1;
    }

    // Save engine to file
    std::cout << "Trying to save engine file now..." << std::endl;
    std::ofstream engineStream(enginePath, std::ios::binary);
    if (!engineStream)
    {
        std::cerr << "Error: Could not open plan output file: " << enginePath
                  << std::endl;
        return 1;
    }

    engineStream.write(static_cast<const char*>(serializedEngine->data()),
                       serializedEngine->size());

    std::cout << "Converted ONNX model to TensorRT engine successfully!"
              << std::endl;
    return 0;
}