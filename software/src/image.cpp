#include "image.hpp"

namespace altf4
{
    // Static Declarations

    // Constructors

    // Methods
    void Image::copyMatrix( cv::Mat* matrix, int channels )
    {
        data.assign(matrix->data, matrix->data + matrix->total() * channels); 
    }
};

