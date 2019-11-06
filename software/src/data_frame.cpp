#include "data_frame.hpp"
#include "tuner.hpp"
#include <opencv2/videoio.hpp>
#include <opencv2/core/mat.hpp>

namespace altf4
{
    // Static Declarations

    // Constructors
    DataFrame::DataFrame( ) 
    { 

    } 

    DataFrame::DataFrame( Image* image ) : image(image)
    {

    }

    // Methods
    void DataFrame::initialize( unsigned int number_of_tests )
    {
        this->binary_images.resize( Tuner::hsv_thresholds.size() );
        this->binary_datas_2d.resize( Tuner::hsv_thresholds.size() );
        this->all_blobs.resize( Tuner::hsv_thresholds.size() );
        this->best_blobs.resize( Tuner::hsv_thresholds.size() );
    }
};

