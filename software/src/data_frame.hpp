#pragma once
#include "image.hpp"
#include "blob.hpp"
#include "datatypes/color.hpp"

#include <opencv2/videoio.hpp>

namespace altf4
{
    class DataFrame
    {
        public:
            // Public Enums

        private:
            // Private Members
            Image* image = nullptr;                                 // Pointer to original image in main.cpp
            std::vector< std::vector< int > > conv_data;            // Convolution data
            std::vector< Image > binary_images;                     // Array of binary_images representing each color ( IE: green / red in our case )
            std::vector< std::vector< Color > > color_2d;           // Array of image colors represented in 2 dimensions
            std::vector< std::vector< unsigned char* > > binary_datas_2d;  // Array of binary_image data represented in 2 dimensions
            std::vector< std::vector< Blob > > all_blobs;           // Array of all blobs found in colored binary_images
            std::vector< Blob > best_blobs;                         // Array of single best blob found in colored binary_image

        public:
            DataFrame();
            DataFrame( Image* image );
            //DataFrame( const DataFrame& other );
            virtual ~DataFrame() { }

            // Methods
            void initialize( unsigned int number_of_tests );

            // Accessors
            Image* getImage() { return this->image; }
            std::vector< std::vector< Color > >& getColor2D() { return this->color_2d; }
            std::vector< std::vector< int > >& getConvData() { return this->conv_data; }
            std::vector< Image >& getBinaryImages() { return this->binary_images; }
            std::vector< std::vector< unsigned char* > >& getBinaryDatas2D() { return this->binary_datas_2d; }
            std::vector< std::vector< Blob > >& getAllBlobs() { return this->all_blobs; }
            std::vector< Blob >& getBestBlobs() { return this->best_blobs; }

            // Mutators
    };
};

