// Header file for anything that needs manual tuning.
#pragma once
#include <vector>
#include "datatypes/color.hpp"

namespace altf4
{
    namespace tuning
    {
/***************************************************************************************/
/**                                     Detection                                     **/
/***************************************************************************************/
        // List of all pixel value Minimums / Maximums for Binary Images
        const std::vector< std::pair< Color, Color > > hsv_thresholds = 
        {
            // Green Laser:
            {
                
                Color{ 50, 50, 50 },   // Min Threshold
                Color{ 200, 200, 200}    // Max Threshold
                //Color{  28,  42, 153 },   // Min Threshold
                //Color{  63, 101, 240 }    // Max Threshold
            },

            // Red Laser:
            {
                Color{ 140,  30, 190 },   // Min Threshold
                Color{ 195, 163, 240 }    // Max Threshold
            },
        };

        const int core_max_neighbors = 3; // Max number of neighbors until not blob anymore
        const int core_size_cutoff = 150; // Bigger than this, and core blob explodes ( prevents blob leak )


/***************************************************************************************/
/**                                      Scoring                                      **/
/***************************************************************************************/
        // List of expected values for scoring ( What the average pixel color should be for the blob )
        const std::vector< Color > hsv_expected_values = 
        {
            // Green Laser:
            {
                Color{ 40, 75, 170 },  
            },

            // Red Laser:
            {
                Color{ 156, 42, 222 },   
            },
        };

        // Scoring features to use 
        const std::vector< bool > scoring_masks = 
        {
            true,   // Closeness to Expected Color ( closer -> higher score )
        };

        // Score percentage cutoff. If above this percentage of max score, then it qualifies
        const float percentage_score_cutoff = 0.8;

/***************************************************************************************/
/**                                      Outputs                                      **/
/***************************************************************************************/
        // List of colors to draw corresponding blobs/text/box/etc to window 
        // Example: 
        //      0: Green-Laser gets green gui elements
        //      1: Red-Laser gets red gui elements
        //         ...
        //      n: ...
        const std::vector< cv::Scalar > associated_color = 
        {
            // Green Laser:
            { 100, 255, 100 },

            // Red Laser:
            { 100, 100, 255 },
        };
    };
};
