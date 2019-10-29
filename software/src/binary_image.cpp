#include "binary_image.hpp"

namespace altf4
{
    // Static Declarations

    // Constructors
    BinaryImage::BinaryImage( int width, int height, int channels )
    {
        this->binary_data.resize( width * height * channels );
    }

    BinaryImage::BinaryImage( std::vector< bool >* binary_data, int width, int height, int channels )
        : Image( width, height, channels )
    {
        this->binary_data = *binary_data;
    }

    // Methods
     
};

