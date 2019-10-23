# Threads
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

set(BASE_SOURCES 
    main.cpp
    serial.cpp
    serial.hpp
)

add_executable(camera ${BASE_SOURCES} ${PONG_SOURCES})
target_link_libraries(camera Threads::Threads)

add_custom_command(TARGET camera
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:camera> "${CMAKE_SOURCE_DIR}/build/"
)
