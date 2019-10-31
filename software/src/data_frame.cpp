#include "data_frame.hpp"
#include "tuning.hpp"
#include <opencv2/videoio.hpp>
#include <opencv2/core/mat.hpp>

namespace altf4
{
    // Static Declarations

    // Constructors
    DataFrame::DataFrame( ) 
    { 
        printf("Creating DataFrame!\n");
    } 

    DataFrame::DataFrame( Image* image ) : image(image)
    {
        printf("Creating DataFrame!\n");
    }

    // Methods
    void DataFrame::initialize( unsigned int number_of_tests )
    {
        this->binary_images.resize( tuning::hsv_thresholds.size() );
        this->binary_datas_2d.resize( tuning::hsv_thresholds.size() );
        this->all_blobs.resize( tuning::hsv_thresholds.size() );
        this->best_blobs.resize( tuning::hsv_thresholds.size() );
    }
};

