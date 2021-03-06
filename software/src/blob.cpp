#include "blob.hpp"

#include "datatypes/pixel.hpp"
#include "datatypes/color.hpp"

namespace altf4
{
    // Static Declarations

    // Constructors

    Blob::Blob( )
    {
        this->initialized = false;
    }

    Blob::Blob( std::vector< Pixel > pixels, Boundary boundary )
        : pixels(pixels), boundary(boundary)
    {
        this->initialized = true;
    }


    // Methods
    cv::Rect Blob::getEncompassingRect( int padding ) 
    { 
        // Note: 
        // For future viewers, OpenCV uses X / Y notation instead of R / C for drawing rectangles.
        // To emphasize this, below is how R / C is translated into X / Y:
        const int blob_min_x = boundary.lb;
        const int blob_min_y = boundary.la;
        const int blob_max_x = boundary.hb;
        const int blob_max_y = boundary.ha;

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
        // Note:  X / Y representation
        return Position{ 
            boundary.lb + ( boundary.hb - boundary.lb ) / 2, // Center Row
            boundary.la + ( boundary.ha - boundary.la ) / 2  // Center Column
        };
    }

    unsigned int Blob::getArea()
    {
        return ( boundary.ha - boundary.la + 1 ) * ( boundary.hb - boundary.lb + 1 );
    }

    // Returns normalized value of how "circular" the overall blob is
    // Just compares width and height
    float Blob::getNormalizedEccentricity()
    {
        int width = boundary.ha - boundary.la + 1; // Plus one to avoid floating point exception
        int height = boundary.hb - boundary.lb + 1; // Plus one to avoid floating point exception
        int maximum = std::max(width, height);
        return 1 - ( std::abs(width - height) / maximum );
    }

    float Blob::getNormalizedCoreArea()
    {
        unsigned int core_area = core.getArea();
        unsigned int blob_area = getArea();
        float ratio = (float)std::min(core_area, blob_area) / (float)blob_area; 
        float ideal = 0.5;
        float multiplier = ideal - (ideal - ratio);

        printf("CoreArea: %d, BlobArea: %d, Ratio: %f\n", core_area, blob_area, multiplier);
        return ratio;

        // TODO 
        // Write blob.getArea()
        // Normalize core area somehow the best you can
        // (Find typical ratio of max area to core area of laser blobs, and base off that)

    }
};

