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

    } 

    DataFrame::DataFrame( Image* image ) : original_image(image)
    {

    }

    // DataFrame::DataFrame( const DataFrame& other ) 
    // {
    //     printf("Copying DataFrame!\n"); 
    //     original_image = other.original_image;
    //     bool temp = other.opencv_matrix.empty() ;
    //     printf("%d\n", temp);
    //     if ( other.opencv_matrix.empty() )
    //     {
    //         opencv_matrix = cv::Mat();
    //     }
    //     else
    //     {
    //         opencv_matrix = other.opencv_matrix.clone();
    //     }
    //     printf("%d\n", temp);
    //     printf("Copied DataFrame!\n"); 
    // }

    // Methods
    void DataFrame::initialize( unsigned int number_of_tests )
    {
        this->binary_images.resize( tuning::hsv_thresholds.size() );
        this->all_blobs.resize( tuning::hsv_thresholds.size() );
        this->best_blobs.resize( tuning::hsv_thresholds.size() );
    }
};

