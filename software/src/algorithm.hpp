#pragma once

#include "image.hpp"
#include "datatypes/color.hpp"
#include <vector>

namespace altf4
{
    namespace algorithm
    {
        void writeBinaryData( Image* image, Image& binary_image, const std::pair< Color, Color >& thresholds );
        void transDimensiate( Image* image, Image& binary_image );
    };
};

