#pragma once
#include "datatypes/pixel.hpp"
#include <vector>

namespace altf4
{
    class Blob
    {
        private:
            // Private Members
            std::vector< Pixel > pixels;
            unsigned int blob_min_row;
            unsigned int blob_min_col;
            unsigned int blob_max_row;
            unsigned int blob_max_col;
            

        public:
            Blob() { }
            Blob( std::vector< Pixel > pixels, 
                unsigned int blob_min_row, unsigned int blob_min_col, unsigned int blob_max_row, unsigned int blob_max_col );
            virtual ~Blob() { }

            // Methods

            // Accessors

            // Mutators
    };
};

