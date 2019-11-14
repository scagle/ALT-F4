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
        void transDimensiateImage( Image* image, std::vector< std::vector< Color > >& data_2d );

        void writeConvData( std::vector< std::vector< Color > >& color_2d, std::vector< std::vector< unsigned char > >& conv_data, 
            std::vector< unsigned char >& conv_data_1d, const std::vector< std::vector< int > >& kernel );

        void writeBinaryData( Image* image, Image& binary_image, const std::pair< Color, Color >& thresholds );

        void transDimensiateBinaryImage( Image& binary_image, std::vector< unsigned char* >& binary_data_2d );

        void getBlobs( Image* image, std::vector< std::vector< Color > >& data_2d, 
                       std::vector< unsigned char* >& binary_data_2d, std::vector< Blob >& all_blobs );

        void scoreBlobs( std::vector< std::vector< Color > >& color_2d, std::vector< unsigned char* >& binary_data_2d, 
            std::vector< Blob >& blobs, Blob& best_blob, int type );

        void convoluteBlob( Blob& blob, std::vector< std::vector< Color > >& color_2d, 
                const std::vector< std::vector< int > >& kernel );
    };
};

