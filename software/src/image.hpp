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

        protected:
            int width, height, channels;
            bool initialized = true; // Assume this gets initialized correctly, unless Image() is called
            

        public:
            Image();
            Image( int width, int height, int channels );
            Image( cv::Mat* matrix, int width, int height, int channels ); 
            Image( const Image& );
            virtual ~Image() { }

            // Methods
            void copyMatrix( cv::Mat* matrix, int channels );
            void convertToVector( cv::Mat* matrix );

            // Accessors
            int getRows() const { return height; }
            int getCols() const { return width; }
            int getChannels() const { return channels; }
            virtual std::vector< unsigned char >* getData() { return &data; } ;
            virtual bool isEmpty() const { return ( this->data.size() == 0 ); }

            // Mutators
    };
};

