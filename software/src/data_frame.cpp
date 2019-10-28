#include "data_frame.hpp"
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

    DataFrame::DataFrame( Image* image ) 
    {
        printf("Creating DataFrame!\n"); 
        original_image = image;
        opencv_matrix = cv::Mat( image->getRows(), image->getCols(), CV_8UC3, image->getData() );
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
    cv::Mat* DataFrame::getMat()
    {
        return &opencv_matrix;
    }
    
};

