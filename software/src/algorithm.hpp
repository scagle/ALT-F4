#pragma once

#include "image.hpp"
#include "blob.hpp"
#include "datatypes/color.hpp"
#include <vector>

namespace altf4
{
    namespace algorithm
    {
        // Algorithms
        void writeBinaryData( Image* image, Image& binary_image, const std::pair< Color, Color >& thresholds );
        void transDimensiateImage( Image* image, std::vector< std::vector< Color > >& data_2d );
        void transDimensiateBinaryImage( Image& binary_image, std::vector< unsigned char* >& binary_data_2d );

        void getBlobs( Image* image, std::vector< std::vector< Color > >& data_2d, 
                       std::vector< unsigned char* >& binary_data_2d, std::vector< Blob >& all_blobs );
    };
};

