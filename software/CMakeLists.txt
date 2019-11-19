cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)

project(camera)

# For GDB Debugging
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
set(CMAKE_BUILD_TYPE RelWithDebInfo)
# Add -O0 to remove optimizations when using gcc in debug mode
IF(CMAKE_COMPILER_IS_GNUCC) 
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

# Finding Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_subdirectory(src)
