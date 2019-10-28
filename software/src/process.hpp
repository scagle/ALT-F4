#pragma once
#include "image.hpp"
#include "data_frame.hpp"

namespace altf4
{
    class Process
    {
        public:
            // Public Enums

        private:
            // Private Members
            Image* original_image;
            DataFrame image_frame;
            

        public:
            Process();
            Process( Image* image );
            virtual ~Process() { printf("Destroying Process!\n"); }

            // Methods
            void processImage();
            DataFrame* getDataFrame();

            // Accessors

            // Mutators
    };
};

