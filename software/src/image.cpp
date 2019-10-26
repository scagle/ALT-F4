#include "image.hpp"

namespace altf4
{
    // Static Declarations

    // Constructors
    Image::Image( int width, int height, int channels ) : width(width), height(height), channels(channels) 
    { 
        data.resize(width * height * channels);  
    }

    Image::Image( Image* other )
    {
        printf("Trying to copy image\n");
        width = other->width;
        height = other->height;
        channels = other->channels;
        data = other->data;
    }


    // Methods
    void Image::copyMatrix( cv::Mat* matrix, int channels )
    {
        data.assign(matrix->data, matrix->data + matrix->total() * channels); 
    }
};

