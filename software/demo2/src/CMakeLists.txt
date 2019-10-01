add_subdirectory(pong)

set(CAMERA_SOURCES 
    blob.cpp
    blob.hpp
    frame.cpp
    frame.hpp
    global.hpp
    main.cpp
    pixel.cpp
    pixel.hpp
)

add_executable(camera ${CAMERA_SOURCES} ${PONG_SOURCES})
target_link_libraries(camera ${OpenCV_LIBS})

message("Copying binary to <project>/build")
add_custom_command(TARGET camera
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:camera> "${CMAKE_SOURCE_DIR}/build/"
)
