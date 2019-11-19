#include "tuner.hpp"

namespace altf4
{
/***************************************************************************************/
/**                                     Detection                                     **/
/***************************************************************************************/
    // Kernel used in edge detection
    const std::vector< std::vector< int > > Tuner::convolution_kernel = 
    {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1},
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
    const int Tuner::number_of_whitest_pixel_searches = 20; // How many "max" pixels we search for before averaging O(kn)


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

    // List of expected areas for scoring ( How big the blob should be )
    const std::vector< unsigned int > Tuner::expected_areas = 
    {
        // Green Laser:
        90,

        // Red Laser:
        70,
    };

    // List of expected areas for scoring ( How big the blob should be )
    const std::vector< unsigned int > Tuner::expected_sizes = 
    {
        // Green Laser:
        100,

        // Red Laser:
        30,
    };

    // List of expected core color values for scoring ( Average color of core of blob )
    const std::vector< Color > Tuner::expected_core_colors = 
    {
        // Green Laser:
        {
            Color{ 255, 80, 90 },  
        },

        // Red Laser:
        {
            Color{ 255, 80, 90 },   
        },
    };

    // List of expected convolution averages for scoring ( How edgey the blob is )
    const std::vector< unsigned char > Tuner::expected_conv_averages = 
    {
        // Green Laser:
        180,

        // Red Laser:
        180,
    };

    // Scoring features to use 
    const std::vector< bool > Tuner::scoring_masks = 
    {
        true,   // Closeness to Expected Color ( closer -> higher score )
        true,   // Closeness to Expected Area  ( closer -> higher score )
        false,   // Closeness to Expected Size  ( closer -> higher score )
    };

    // Scoring features to use on high scoring blobs
    const std::vector< bool > Tuner::scoring_rigorous_masks = 
    {
        true,   // Closeness to expected Anchor Color ( closer -> higher score )
        false,  // Expected Core Anchor length ( closer -> higher score )
        true,   // Exploded Core Anchor ( exploded -> bad score )
        true,   // Closeness to expected Convolution Average ( closer -> higher score )
    };

    // Score percentage cutoff. If above this percentage of max score, then it qualifies
    const float Tuner::percentage_score_cutoff = 0.7;

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

