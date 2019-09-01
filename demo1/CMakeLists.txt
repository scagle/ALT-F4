cmake_minimum_required(VERSION 3.1)
project(camera VERSION 1.0.0)

# Set to where opencv libraries are installed
#set(OpenCV_DIR "/usr/lib/")

# For GDB Debugging
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_BUILD_TYPE RelWithDebInfo)

# Find packages of opencv
#find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui videoio features2d)

# Add executable and link libraries to it
add_executable(camera main.cpp)

# Find packages for gstreamer
find_package(PkgConfig)
pkg_check_modules(GST REQUIRED glib-2.0)
include_directories(${GLIB_INCLUDE_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})
pkg_check_modules(GST REQUIRED gstreamer-1.0)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-app-1.0)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-audio-1.0)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-pbutils-1.0)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-fft-1.0)

set(ENV{GST_PLUGIN_PATH} "/usr/lib/gstreamer-1.0/")
target_include_directories(camera PUBLIC ${GST_INCLUDE_DIRS})
target_compile_options(camera PUBLIC ${GST_CFLAGS})
target_link_libraries(camera ${GST_LIBRARIES})

