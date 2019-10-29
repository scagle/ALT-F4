#include "image.hpp"
#include "globals.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>

namespace altf4
{
    // Static Declarations

    // Constructors
    Image::Image()
    {

    }

    Image::Image( int rows, int cols, int channels ) : rows(rows), cols(cols), channels(channels) 
    { 
        data.resize(rows * cols * channels);  
    }

    Image::Image( cv::Mat* matrix, int rows, int cols, int channels ) : rows(rows), cols(cols), channels(channels) 
    { 
        data.resize(rows * cols * channels);  
        convertToVector(matrix);
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

