# Set to where opencv libraries are installed
set(OpenCV_DIR "/usr/lib/")

# Find packages of opencv
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui videoio features2d)
include_directories(${OpenCV_INCLUDE_DIRS})

# Threads
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

set(BASE_SOURCES 
    blob.cpp
    blob.hpp
    frame.cpp
    frame.hpp
    global.hpp
    main.cpp
    pixel.cpp
    pixel.hpp
    serial.cpp
    serial.hpp
)

add_subdirectory(pong)

add_executable(camera ${BASE_SOURCES} ${PONG_SOURCES})
target_link_libraries(camera ${OpenCV_LIBS})
target_link_libraries(camera Threads::Threads)

add_custom_command(TARGET camera
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:camera> "${CMAKE_SOURCE_DIR}/build/"
)
