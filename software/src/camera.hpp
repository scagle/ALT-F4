#pragma once
#include "image.hpp"
#include <opencv2/videoio.hpp>

namespace altf4
{
    class Camera
    {
        public:
            // Public Enums

        private:
            // Private Members
            cv::VideoCapture cap;    // open the camera located at /dev/videoX
            cv::Mat3b matrix_buffer;    // open the camera located at /dev/videoX
            std::pair< cv::Mat3b, Image > image_pair;

            int camera_number;
            int channels;
            int rows;
            int cols;

        public:
            Camera() { }
            virtual ~Camera() { }

            // Methods
            std::pair< cv::Mat3b, Image >* grabImage();
            bool initialize( int camera_number );
            void printStats();

            // Accessors

            // Mutators
    };
};

