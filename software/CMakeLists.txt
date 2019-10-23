cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)

project(camera)

# For GDB Debugging
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
set(CMAKE_BUILD_TYPE RelWithDebInfo)

# Finding Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_subdirectory(src)
