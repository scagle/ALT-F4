#pragma once
#include "image.hpp"
#include <vector>

namespace altf4
{
    class BinaryImage : public Image
    {
        private:
        // Private Members
        std::vector< unsigned char > data;

        public:
        BinaryImage() { }
        BinaryImage( int width, int height, int channels );         
        BinaryImage( std::vector< bool >* binary_data, int width, int height, int channels );
        virtual ~BinaryImage() { }

        // Methods

        // Accessors
        std::vector< unsigned char >* getData() const override{ return &data; } ;
        bool isEmpty() const override { return ( this->binary_data.size() == 0 ); }

        // Mutators
    };
};

