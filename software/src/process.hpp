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
            int camera_index;
            Image& image;
            DataFrame& frame;
            

        public:
            Process( int camera_index, Image& image, DataFrame& frame );
            virtual ~Process() { printf("Destroying Process!\n"); }

            // Methods
            void performAlgorithms();

            // Accessors
            DataFrame& getDataFrame() { return this->frame; }

            // Mutators
    };
};

