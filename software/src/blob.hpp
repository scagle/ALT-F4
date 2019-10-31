#pragma once
#include "datatypes/pixel.hpp"
#include "datatypes/color.hpp"
#include "core.hpp"
#include <vector>
#include <opencv2/videoio.hpp>

namespace altf4
{
    class Blob
    {
        private:
            // Private Members
            std::vector< Pixel > pixels;
            std::vector< Pixel > core_pixels;
            Core core;
            int blob_min_y;
            int blob_min_x;
            int blob_max_y;
            int blob_max_x;
            float score;
            bool initialized = false; // If blob is created but not initialized
            bool exploded = false; // If blob exceeds max_size when it is being built, it explodes and is useless  ( blob_detection )

        public:
            Blob(); 
            Blob( std::vector< Pixel > pixels, int blob_min_y, int blob_min_x, int blob_max_y, int blob_max_x );
            virtual ~Blob() { }

            // Methods
            cv::Rect getEncompassingRect( int padding );
            Color getAverageColor();
            int getArea(){ return ( ( blob_max_x - blob_min_x ) * ( blob_max_y - blob_min_y ) ); }
            Position getCenterPosition();
            void setScore( int score ) { this->score = score; }

            // Accessors
            std::vector< Pixel >* getPixels() { return &( this->pixels ); }
            bool isInitialized() { return this->initialized; }
            bool isExploded() { return this->exploded; }
            std::vector< Pixel >* getCorePixels() { return &(this->core_pixels); }
            Core* getCore() { return &(this->core); }

            // Mutators
            void explode() { this->exploded = true; }  // Max-size exceeded. Blob exploded... ( blob_detection )
            void setCorePixels( std::vector< Pixel > core_pixels ) { this->core_pixels = core_pixels; }
            void setCore( Core core ) { this->core = core; }
    };
};

