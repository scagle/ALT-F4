#pragma once
#include "image.hpp"
#include <opencv2/videoio.hpp>

namespace altf4
{
    class DataFrame
    {
        public:
            // Public Enums

        private:
            // Private Members
            Image* original_image;
            cv::Mat opencv_matrix;  // opencv matrix for displaying
            

        public:
            DataFrame();
            DataFrame( Image* image );
            //DataFrame( const DataFrame& other );
            virtual ~DataFrame() 
            { 
                printf("Destroying DataFrame!\n"); 
            }

            // Methods
            cv::Mat* getMat();

            // Accessors

            // Mutators
    };
};

