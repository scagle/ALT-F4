#include "image.hpp"
#include "globals.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>

namespace altf4
{
    // Static Declarations

    // Constructors
    Image::Image( int width, int height, int channels ) : width(width), height(height), channels(channels) 
    { 
        data.resize(width * height * channels);  
    }

    Image::Image( cv::Mat* matrix, int width, int height, int channels ) : width(width), height(height), channels(channels) 
    { 
        data.resize(width * height * channels);  
        convertToVector(matrix);
    }

    Image::Image( const Image& other )
    {
        width = other.width;
        height = other.height;
        channels = other.channels;
        data = other.data;
    }


    // Methods
    void Image::copyMatrix( cv::Mat* matrix, int channels )
    {
        convertToVector( matrix );
    }

    void Image::convertToVector( cv::Mat* matrix )
    {
        if ( matrix->isContinuous() ) 
        {
            data.assign(matrix->data, matrix->data + matrix->total() * channels);
        } 
        else 
        {
            printf("*** WARNING: Image is not continuous! (image.cpp)\n");
            //for (int i = 0; i < matrix->rows; ++i) 
            //{
            //    data.insert(data.end(), matrix->ptr<uchar>(i), matrix->ptr<uchar>(i)+matrix->cols);
            //}
        }
    }
};

