cmake_minimum_required(VERSION 3.10)
project(Converter CUDA CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CUDA REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(
    ${CUDA_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    /usr/include/aarch64-linux-gnu
)

add_executable(Converter converter.cpp)

target_link_libraries(Converter
    ${OpenCV_LIBS}
    ${CUDA_LIBRARIES}
    nvinfer
    nvonnxparser
)