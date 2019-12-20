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
            int rows, cols, channels;
            bool initialized = true; // Assume this gets initialized correctly, 
                                     // unless Image() is called

        public:
            Image();
            Image( int rows, int cols, int channels );
            Image( cv::Mat* matrix, int rows, int cols, int channels ); 
            virtual ~Image() { }

            // Methods
            void copyMatrix( cv::Mat* matrix, int channels );
            void convertToVector( cv::Mat* matrix );

            // Accessors
            int getRows() const { return rows; }
            int getCols() const { return cols; }
            int getChannels() const { return channels; }
            virtual std::vector< unsigned char >* getData() { return &data; } ;
            virtual bool isEmpty() const { return ( this->data.size() == 0 ); }

            // Mutators
    };
};

