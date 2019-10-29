#pragma once
#include "camera.hpp"
#include "image.hpp"
#include <vector>
#include <thread>
#include <mutex>
#include <deque>

namespace altf4
{
    class CameraHandler
    {
        public:
            // Public Enums
            // Private Members
            static std::vector< Camera > cameras;              // List of cameras
            static std::vector< std::thread > camera_threads;  // Threads used to grab images from OpenCV
            static std::vector< Image > images;                // 
            static std::mutex* print_lock;                     // Locks stdout
            static std::mutex image_lock;                      // To protect images
            //static std::mutex fps_lock;                      // To protect image_copies
            static bool stop_threads;  
            static std::vector< bool > updated_list;
            static std::vector< bool > working_list;
            //static std::vector< std::deque< int > > fps_timing;

        private:

        public:
            CameraHandler() { } 
            virtual ~CameraHandler() { }

            // Methods
            static void grabVideoFramesThread( unsigned int camera_index );
            std::vector< Image > readImages(); // Grabs a synchronized copy of images
            void resolveThreads();
            bool initialize( unsigned int num_cam, std::mutex* pl);
            bool imagesReady();

            // Accessors

            // Mutators
    };
};

