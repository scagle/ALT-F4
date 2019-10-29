#include "blob.hpp"

namespace altf4
{
    // Static Declarations

    // Constructors
    Blob::Blob( std::vector< Pixel > pixels, 
        unsigned int blob_min_row, unsigned int blob_min_col, unsigned int blob_max_row, unsigned int blob_max_col )
        : pixels(pixels), blob_min_row(blob_min_row), blob_min_col(blob_min_col), blob_max_row(blob_max_row), blob_max_col(blob_max_col)
    {

    }


    // Methods
    
};

