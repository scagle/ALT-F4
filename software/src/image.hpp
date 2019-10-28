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
            cv::Mat matrix;
            int width, height, channels;
            

        public:
            Image() { }
            Image( int width, int height, int channels );
            Image( const Image& );
            virtual ~Image() { }

            // Methods
            void copyMatrix( cv::Mat* matrix, int channels );
            void convertToVector( cv::Mat* matrix );

            // Accessors
            int getRows() const { return height; }
            int getCols() const { return width; }
            int getChannels() const { return channels; }
            std::vector< unsigned char >* getData() { return &data; } ;
            cv::Mat* getMatrix() { return &matrix; } ;

            // Mutators
    };
};

