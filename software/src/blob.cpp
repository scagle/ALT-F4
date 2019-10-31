#include "blob.hpp"

namespace altf4
{
    // Static Declarations

    // Constructors

    Blob::Blob( )
    {
        this->initialized = false;
    }

    Blob::Blob( std::vector< Pixel > pixels, int blob_min_y, int blob_min_x, int blob_max_y, int blob_max_x )
        : pixels(pixels), blob_min_y(blob_min_y), blob_min_x(blob_min_x), blob_max_y(blob_max_y), blob_max_x(blob_max_x)
    {
        this->initialized = true;
    }


    // Methods
    cv::Rect Blob::getEncompassingRect( int padding ) 
    { 
        return cv::Rect(    blob_min_x - padding, 
                            blob_min_y - padding, 
                            ( blob_max_x - blob_min_x ) + ( padding * 2 ), 
                            ( blob_max_y - blob_min_y ) + ( padding * 2 )
                       ); 
    }

    Color Blob::getAverageColor()
    {
        // get average BGR/HSV
        unsigned int avg_c1 = 0;
        unsigned int avg_c2 = 0;
        unsigned int avg_c3 = 0;
        for ( auto&& pixel : pixels )
        {
            avg_c1 += pixel.color.b;
            avg_c2 += pixel.color.g;
            avg_c3 += pixel.color.r;
        }
        avg_c1 /= pixels.size();
        avg_c2 /= pixels.size();
        avg_c3 /= pixels.size();

        return Color{(unsigned char)avg_c1, (unsigned char)avg_c2, (unsigned char)avg_c3};
    }

    Position Blob::getCenterPosition()
    {
        return Position{ 
            blob_min_y + ( blob_max_y - blob_min_y ) / 2, // Center Row
            blob_min_x + ( blob_max_x - blob_min_x ) / 2  // Center Column
        };
    }
};

