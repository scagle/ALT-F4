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
    camera.cpp
    camera_handler.cpp
    camera_handler.hpp
    camera.hpp
    data_frame.cpp
    data_frame.hpp
    globals.hpp
    image.cpp
    image.hpp
    keyboard_handler.cpp
    keyboard_handler.hpp
    main.cpp
    process.cpp
    process_handler.cpp
    process_handler.hpp
    process.hpp
    serial_handler.cpp
    serial_handler.hpp
    window.cpp
    window.hpp
)

add_executable(camera ${BASE_SOURCES} ${PONG_SOURCES})
target_link_libraries(camera ${OpenCV_LIBS})
target_link_libraries(camera Threads::Threads)

add_custom_command(TARGET camera
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:camera> "${CMAKE_SOURCE_DIR}/build/"
)
