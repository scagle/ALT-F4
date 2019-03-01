cmake_minimum_required(VERSION 3.13)
project(webcam VERSION 1.0.0)

# Set to where opencv libraries are installed
set(OpenCV_DIR "/usr/lib/")

# Find packages of opencv
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)
include_directories(${OpenCV_INCLUDE_DIRS})

# Add executable and link libraries to it
add_executable(webcam main.cpp)
target_link_libraries(webcam ${OpenCV_LIBS})
