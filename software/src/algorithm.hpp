#pragma once

#include "image.hpp"
#include "datatypes/color.hpp"
#include <vector>

namespace altf4
{
    namespace algorithm
    {
        void writeBinaryData( Image* image, Image& binary_image, const std::pair< Color, Color >& thresholds );
        void transDimensiateBinaryMatrix( Image& binary_image, std::vector< unsigned char* >& binary_data_2d );
    };
};

