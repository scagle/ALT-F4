// Header file for anything that needs manual tuning.
#pragma once
#include <vector>
#include <opencv2/videoio.hpp>
#include "datatypes/color.hpp"

namespace altf4
{
    class Tuner
    {
        public:
/***************************************************************************************/
/**                                     Detection                                     **/
/***************************************************************************************/

    // Kernel used in edge detection
    static const std::vector< std::vector< int > > convolution_kernel; 

    // List of all pixel value Minimums / Maximums for Binary Images
    static std::vector< std::pair< Color, Color > > hsv_thresholds;

    // Recursive Approach
    //static const int core_max_neighbors; // Max number of neighbors until not blob anymore
    //static const int core_size_cutoff; // Bigger than this, and core blob explodes ( prevents blob leak )

    // Sticky Ball Approach
    static const int number_of_whitest_pixel_searches; // How many "max" pixels we search for before averaging O(kn)


/***************************************************************************************/
/**                                      Scoring                                      **/
/***************************************************************************************/
    // List of expected values for scoring ( What the average pixel color should be for the blob )
    static const std::vector< Color > hsv_expected_values;

    // List of expected areas for scoring ( How big the blob should be )
    static const std::vector< unsigned int > expected_areas;

    // List of expected sizes for scoring ( How many pixels the blob should have )
    static const std::vector< unsigned int > expected_sizes;

    // List of expected core color values for scoring ( Average color of core of blob )
    static const std::vector< Color > expected_core_colors;

    // List of expected core anchor length for scoring (average CoreAnchor lengths)
    static const std::vector< unsigned int > expected_core_length;

    // List of expected convolution averages for scoring ( How edgey the blob is )
    static const std::vector< unsigned char > expected_conv_averages;

    // Scoring features to use 
    static const std::vector< bool > scoring_masks;

    // Scoring features to use on high scoring blobs
    static const std::vector< bool > scoring_rigorous_masks;

    // Score percentage cutoff. If above this percentage of max score, then it qualifies
    static const float percentage_score_cutoff;

/***************************************************************************************/
/**                                      Outputs                                      **/
/***************************************************************************************/
    // List of colors to draw corresponding blobs/text/box/etc to window 
    // Example: 
    //      0: Green-Laser gets green gui elements
    //      1: Red-Laser gets red gui elements
    //         ...
    //      n: ...
    static const std::vector< cv::Scalar > associated_color;
    };
};
