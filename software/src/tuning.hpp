// Header file for anything that needs manual tuning.
#pragma once
#include <vector>
#include "datatypes/color.hpp"

namespace altf4
{
    namespace tuning
    {
        // List of all pixel value Minimums / Maximums for Binary Images
        const std::vector< std::pair< Color, Color > > hsv_thresholds = 
        {
            // Green Laser:
            {
                
                Color{  28,  42, 153 },   // Min Threshold
                Color{ 255, 255, 255 }    // Max Threshold
            },

            // Red Laser:
            {
                Color{ 140,  30, 190 },   // Min Threshold
                Color{ 195, 163, 240 }    // Max Threshold
            },
        };


        // List of colors to draw corresponding blobs/text/box/etc to window 
        // Example: 
        //      0: Green-Laser gets green gui elements
        //      1: Red-Laser gets red gui elements
        //         ...
        //      n: ...
        const std::vector< Color > associated_color = 
        {
            // Green Laser:
            Color{ 100, 255, 100 },

            // Red Laser:
            Color{ 100, 100, 255 },
        };
    };
};
