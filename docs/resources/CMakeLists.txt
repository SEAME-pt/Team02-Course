cmake_minimum_required(VERSION 3.10)
project(Converter CUDA CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -fopenmp -ffast-math")

enable_language(CUDA)
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -O3 --use_fast_math --gpu-architecture=sm_72")

set(CUDA_TOOLKIT_ROOT_DIR /usr/local/cuda)

find_package(CUDA REQUIRED)
find_package(OpenCV REQUIRED)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

include_directories(
    ${CUDA_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    /usr/include/aarch64-linux-gnu
)

add_executable(Controller controller.cpp)

find_library(CUDA_RUNTIME_LIBRARY
    NAMES cudart
    HINTS ${CUDA_TOOLKIT_ROOT_DIR}/lib64
)

target_link_libraries(Controller
    ${OpenCV_LIBS}
    ${CUDA_LIBRARIES}
    ${CUDA_RUNTIME_LIBRARY}
    nvinfer
    nvonnxparser
)

add_executable(Converter converter.cpp)

target_link_libraries(Converter
    ${OpenCV_LIBS}
    ${CUDA_LIBRARIES}
    nvinfer
    nvonnxparser
    OpenMP::OpenMP_CXX
)