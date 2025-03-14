

std::shared_ptr<nvinfer1::IExecutionContext>
YOLOv10::createExecutionContext(const std::string& modelPath)
{
    // Open the model file in binary mode
    std::ifstream filePtr(modelPath, std::ios::binary);

    // Check if the file was opened successfully
    if (!filePtr.good())
    {
        std::cerr << "File cannot be opened, please check the file!"
                  << std::endl;
        return nullptr; // Return nullptr if the file cannot be opened
    }

    // Determine the size of the file
    size_t size = 0;
    filePtr.seekg(0, filePtr.end); // Move to the end of the file
    size =
        filePtr
            .tellg(); // Get the current position, which is the size of the file
    filePtr.seekg(0, filePtr.beg); // Move back to the beginning of the file

    // Allocate memory to hold the file contents
    char* modelStream = new char[size];
    filePtr.read(modelStream,
                 size); // Read the entire file into the allocated memory
    filePtr.close();    // Close the file after reading

    // Create an instance of nvinfer1::IRuntime
    nvinfer1::IRuntime* runtime = nvinfer1::createInferRuntime(logger);
    if (!runtime)
    {
        std::cerr << "Failed to create runtime" << std::endl;
        delete[] modelStream; // Free the allocated memory
        return nullptr;       // Return nullptr if the runtime creation fails
    }

    // Deserialize the model and create an ICudaEngine
    nvinfer1::ICudaEngine* engine =
        runtime->deserializeCudaEngine(modelStream, size);
    delete[] modelStream; // Free the allocated memory
    if (!engine)
    {
        std::cerr << "Failed to create engine" << std::endl;
        runtime->destroy(); // Clean up runtime
        return nullptr;     // Return nullptr if the engine creation fails
    }

    // Create an execution context from the engine
    nvinfer1::IExecutionContext* context = engine->createExecutionContext();
    if (!context)
    {
        std::cerr << "Failed to create execution context" << std::endl;
        engine->destroy(); // Clean up engine
        return nullptr;    // Return nullptr if the execution context creation
                           // fails
    }

    // Return a shared pointer to the execution context with a custom deleter
    return std::shared_ptr<nvinfer1::IExecutionContext>(
        context,
        [](nvinfer1::IExecutionContext* ctx)
        {
            ctx->destroy(); // Clean up the execution context
        });
}

void YOLOv10::inferVideo(const std::string& videoPath,
                         const std::string& enginePath)
{
    // Create a shared pointer to the execution context using the provided
    // engine path
    std::shared_ptr<nvinfer1::IExecutionContext> context =
        createExecutionContext(enginePath);

    // Open the video file using OpenCV's VideoCapture
    cv::VideoCapture capture(videoPath);

    // Check if the video file was opened successfully
    if (!capture.isOpened())
    {
        std::cerr << "ERROR: Could not open video file." << std::endl;
        return; // Exit if the video file cannot be opened
    }

    // Create a CUDA stream for asynchronous operations
    cudaStream_t stream;
    cudaStreamCreate(&stream);

    // Allocate device memory for input and output data
    void* inputSrcDevice;
    void* outputSrcDevice;
    cudaMalloc(
        &inputSrcDevice,
        3 * 640 * 640 *
            sizeof(float)); // For storing input data (3 channels, 640x640 size)
    cudaMalloc(&outputSrcDevice,
               1 * 300 * 6 * sizeof(float)); // For storing output data (300
                                             // detections, each with 6 values)

    // Create vectors to hold input and output data on the host (CPU)
    std::vector<float> output_data(
        300 * 6); // Buffer to hold output data from the device
    std::vector<float> inputData(
        640 * 640 *
        3); // Buffer to hold input data (640x640 image with 3 channels)

    // Process video frames in a loop
    while (true)
    {
        cv::Mat frame;

        // Read the next frame from the video file
        if (!capture.read(frame))
        {
            break; // Exit the loop if no more frames are available
        }

        float factor = 0;
        // Preprocess the frame (resize, normalize, etc.) and store the result
        // in inputData
        preProcess(&frame, 640, &factor, inputData);

        // Copy the preprocessed input data from host to device memory
        cudaMemcpyAsync(inputSrcDevice, inputData.data(),
                        3 * 640 * 640 * sizeof(float), cudaMemcpyHostToDevice,
                        stream);

        // Set up bindings for TensorRT inference (input and output tensors)
        void* bindings[] = {inputSrcDevice, outputSrcDevice};

        // Perform inference using the TensorRT execution context
        context->enqueueV2(bindings, stream, nullptr);

        // Copy the output data from device to host memory
        cudaMemcpyAsync(output_data.data(), outputSrcDevice,
                        300 * 6 * sizeof(float), cudaMemcpyDeviceToHost,
                        stream);

        // Wait for the CUDA stream operations to complete
        cudaStreamSynchronize(stream);

        // Post-process the output data to extract detection results
        std::vector<DetResult> result =
            postProcess(output_data.data(), factor, 300);

        // Draw bounding boxes and annotations on the frame
        drawBbox(frame, result);

        // Display the annotated frame in a window
        cv::imshow("RESULT", frame);

        // Wait for 10 milliseconds or until a key is pressed
        cv::waitKey(10);
    }

    // Destroy all OpenCV windows after processing is complete
    cv::destroyAllWindows();

    // Free the allocated CUDA memory and destroy the CUDA stream
    cudaFree(inputSrcDevice);
    cudaFree(outputSrcDevice);
    cudaStreamDestroy(stream);
}
