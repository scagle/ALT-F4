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
        this->initialized = false;
    } 

    DataFrame::DataFrame( int camera_index, Image* image ) : 
        camera_index(camera_index), image(image)
    {
        this->initialized = true;
    }

    // Methods
    void DataFrame::initialize( int camera_index, unsigned int number_of_tests )
    {
        this->initialized = true;
        this->camera_index = camera_index;
        this->binary_images.resize( Tuner::hsv_thresholds[0].size() );
        this->binary_datas_2d.resize( Tuner::hsv_thresholds[0].size() );
        this->all_blobs.resize( Tuner::hsv_thresholds[0].size() );
        this->best_blobs.resize( Tuner::hsv_thresholds[0].size() );
    }
};

