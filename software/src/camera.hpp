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
            cv::VideoCapture cap;  // open the camera located at /dev/videoX
            cv::Mat matrix_buffer; // store camera captures in here initialially

            int camera_number;
            int channels;
            int cols;
            int width;
            int rows;
            int height;

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

