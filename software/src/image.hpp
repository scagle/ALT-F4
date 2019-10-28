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
            Image( const Image& );
            virtual ~Image() { }

            // Methods
            void copyMatrix( cv::Mat* matrix, int channels );

            // Accessors
            int getRows() const { return height; }
            int getCols() const { return width; }
            int getChannels() const { return channels; }
            std::vector< unsigned char >* getData() { return &data; } ;

            // Mutators
    };
};

