#pragma once
#include "image.hpp"
#include "blob.hpp"

#include <opencv2/videoio.hpp>

namespace altf4
{
    class DataFrame
    {
        public:
            // Public Enums

        private:
            // Private Members
            Image* original_image = nullptr;                         // Pointer to original image in main.cpp
            std::vector< Image > binary_images;            // Array of binary_images representing each color ( IE: green / red in our case )
            std::vector< std::vector< Blob > > all_blobs;  // Array of all blobs found in colored binary_images
            std::vector< Blob >                best_blobs; // Array of single best blob found in colored binary_image

        public:
            DataFrame();
            DataFrame( Image* image );
            //DataFrame( const DataFrame& other );
            virtual ~DataFrame() { printf("Destroying DataFrame!\n"); }

            // Methods
            void initialize( unsigned int number_of_tests );

            // Accessors
            Image* getOriginalImage() { return this->original_image; }
            std::vector< Image >& getBinaryImages() { return this->binary_images; }
            std::vector< std::vector< Blob > >& getAllBlobs() { return this->all_blobs; }
            std::vector< Blob >& getBestBlobs() { return this->best_blobs; }

            // Mutators
    };
};

