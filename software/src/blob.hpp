#pragma once
#include "datatypes/pixel.hpp"
#include <vector>
#include <opencv2/videoio.hpp>

namespace altf4
{
    class Blob
    {
        private:
            // Private Members
            std::vector< Pixel > pixels;
            unsigned int blob_min_row;
            unsigned int blob_min_col;
            unsigned int blob_max_row;
            unsigned int blob_max_col;
            

        public:
            Blob() { }
            Blob( std::vector< Pixel > pixels, 
                unsigned int blob_min_row, unsigned int blob_min_col, unsigned int blob_max_row, unsigned int blob_max_col );
            virtual ~Blob() { }

            // Methods

            // Accessors
            cv::Rect getRect() { return cv::Rect( blob_min_row, blob_min_col, blob_max_row, blob_max_col ); }
            cv::Rect getEncompassingRect( int padding ) 
            { return cv::Rect( blob_min_row - padding, blob_min_col - padding, blob_max_row + padding, blob_max_col + padding ); }

            // Mutators
    };
};

