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
    const std::string onnxFile   = "model_segmentation-128-256.onnx";
    const std::string enginePath = "model_segmentation-128-256.engine";

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

    config->setMaxWorkspaceSize(256 << 20); // 256MB instead of 1GB

    // Enable FP16 for memory efficiency
    if (builder->platformHasFastFp16())
    {
        std::cout << "Enabling FP16 precision..." << std::endl;
        config->setFlag(nvinfer1::BuilderFlag::kFP16);
    }

    // Add memory optimization flags
    config->setFlag(nvinfer1::BuilderFlag::kTF32); // Enable TF32 computation
    config->setFlag(nvinfer1::BuilderFlag::kSPARSE_WEIGHTS);

    // Set default device type to GPU
    config->setDefaultDeviceType(nvinfer1::DeviceType::kGPU);

    // Only set DLA core if available
    if (builder->getNbDLACores() > 0)
    {
        config->setDLACore(0);
    }

    nvinfer1::IOptimizationProfile* profile =
        builder->createOptimizationProfile();

    // Set dimensions matching your ONNX model
    nvinfer1::Dims4 dims(1, 3, 128, 256);
    profile->setDimensions("input", // Input tensor name from ONNX model
                           nvinfer1::OptProfileSelector::kMIN, dims);
    profile->setDimensions("input", nvinfer1::OptProfileSelector::kOPT, dims);
    profile->setDimensions("input", nvinfer1::OptProfileSelector::kMAX, dims);

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