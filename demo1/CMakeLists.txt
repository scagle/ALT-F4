cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)

project(camera)

# Source files
file(   GLOB SOURCES
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.h"
        "${PROJECT_SOURCE_DIR}/src/pong/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/pong/*.h"
    )


# Set to where opencv libraries are installed
set(OpenCV_DIR "/usr/lib/")

# For GDB Debugging
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_BUILD_TYPE RelWithDebInfo)

# Find packages of opencv
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui videoio features2d)
include_directories(${OpenCV_INCLUDE_DIRS})

# Add executable and link libraries to it
add_executable(camera ${SOURCES})
target_link_libraries(camera ${OpenCV_LIBS})
