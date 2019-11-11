#include "tuner.hpp"

namespace altf4
{
/***************************************************************************************/
/**                                     Detection                                     **/
/***************************************************************************************/
    // Kernel used in edge detection
    const std::vector< std::vector< int > > Tuner::convolution_kernel = 
    {
        { 0, -1,  0},
        {-1,  4, -1},
        { 0, -1,  0},
    };

    // List of all pixel value Minimums / Maximums for Binary Images
    std::vector< std::pair< Color, Color > > Tuner::hsv_thresholds = 
    {
        // Green Laser:
        {
            
            Color{  30,  20, 100 },   // Min Threshold
            Color{  90, 200, 245 }    // Max Threshold
        },

        // Red Laser:
        {
            Color{ 140,  30, 190 },   // Min Threshold
            Color{ 195, 163, 240 }    // Max Threshold
        },
    };

    // Recursive Approach
    //const int Tuner::core_max_neighbors = 3; // Max number of neighbors until not blob anymore
    //const int Tuner::core_size_cutoff = 150; // Bigger than this, and core blob explodes ( prevents blob leak )

    // Sticky Ball Approach
    const int Tuner::number_of_whitest_pixel_searches = 3; // How many "max" pixels we search for before averaging O(kn)


/***************************************************************************************/
/**                                      Scoring                                      **/
/***************************************************************************************/
    // List of expected values for scoring ( What the average pixel color should be for the blob )
    const std::vector< Color > Tuner::hsv_expected_values = 
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
    const std::vector< bool > Tuner::scoring_masks = 
    {
        true,   // Closeness to Expected Color ( closer -> higher score )
    };

    // Score percentage cutoff. If above this percentage of max score, then it qualifies
    const float Tuner::percentage_score_cutoff = 0.8;

/***************************************************************************************/
/**                                      Outputs                                      **/
/***************************************************************************************/
    // List of colors to draw corresponding blobs/text/box/etc to window 
    // Example: 
    //      0: Green-Laser gets green gui elements
    //      1: Red-Laser gets red gui elements
    //         ...
    //      n: ...
    const std::vector< cv::Scalar > Tuner::associated_color = 
    {
        // Green Laser:
        { 100, 255, 100 },

        // Red Laser:
        { 100, 100, 255 },
    };
};

