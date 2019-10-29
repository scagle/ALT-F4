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
            cv::Mat3b matrix_buffer; // store camera captures in here initialially
            Image current_image;

            int camera_number;
            int channels;
            int rows;
            int cols;

        public:
            Camera() { }
            virtual ~Camera() { }

            // Methods
            Image* grabImage();
            bool initialize( int camera_number );
            void printStats();

            // Accessors

            // Mutators
    };
};

