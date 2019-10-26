#pragma once
#include <opencv2/videoio.hpp>

namespace altf4
{
    class Image
    {
        public:
            // Public Enums

        private:
            // Private Members
            std::vector< unsigned char > data;
            int width, height, channels;
            

        public:
            Image() { }
            Image( int width, int height, int channels );
            Image( Image* );
            virtual ~Image() { }

            // Methods
            void copyMatrix( cv::Mat* matrix, int channels );

            // Accessors

            // Mutators
    };
};

