#pragma once
#include "camera.hpp"
#include "image.hpp"
#include <vector>
#include <thread>
#include <mutex>

namespace altf4
{
    class CameraHandler
    {
        public:
            // Public Enums
            // Private Members
            static std::vector< Camera > cameras;                     // List of cameras
            static std::vector< std::thread > camera_threads;         // Threads used to grab images from OpenCV
            static std::vector< std::vector< Image > > image_copies;  // Copies to use for "Double-Buffer" effect
            static std::mutex* print_lock;                            // 
            static std::mutex image_copies_lock;                      // To protect image_copies
            static int current_copies_index;                          // Alternates between 0 and 1 every read
            static bool stop_threads;  

        private:

        public:
            CameraHandler() { } 
            CameraHandler( int number_of_cameras, std::mutex* pl );
            virtual ~CameraHandler() { }

            // Methods
            std::vector< Image >* read();
            void resolveThreads();
            bool initializeCameras();

            // Accessors

            // Mutators
    };
};

