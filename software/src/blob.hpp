#pragma once
#include "datatypes/pixel.hpp"
#include "datatypes/pixel_1.hpp"
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
            std::vector< Pixel_1 > conv_pixels;
            std::vector< Pixel > core_pixels;
            Core core;
            Boundary boundary; // Boundary of blob
            unsigned char score_average_color = 0;
            unsigned char score_area = 0;
            unsigned char score_size = 0;
            unsigned char score_conv_average = 0;
            float percent_score;
            bool initialized = false; // If blob is created but not initialized
            unsigned int conv_sum = 0;
            unsigned char conv_average = 0;

        public:
            Blob(); 
            Blob( std::vector< Pixel > pixels, Boundary boundary );
            virtual ~Blob() { }

            // Methods
            cv::Rect getEncompassingRect( int padding ); // Get SDL_Rect representation of blob
            Color getAverageColor();
            unsigned int getArea();
            unsigned int getSize() { return this->pixels.size(); }
            Position getCenterPosition();
            void setScore( float percent_score ) { this->percent_score = percent_score; }
            float getNormalizedEccentricity();
            float getNormalizedCoreArea();

            // Accessors
            std::vector< Pixel >* getPixels() { return &( this->pixels ); }
            std::vector< Pixel_1 >* getConvPixels() { return &( this->conv_pixels ); }
            bool isInitialized() { return this->initialized; }
            bool isExploded() { return this->core.isExploded(); }
            std::vector< Pixel >* getCorePixels() { return &(this->core_pixels); }
            bool hasCore() { return core.isInitialized(); }
            Boundary* getBoundary() { return &(this->boundary); }
            Core* getCore() { return &(this->core); }
            float getScore() { return this->percent_score; }
            unsigned int getConvolutionSum() { return conv_sum; }
            unsigned char getConvolutionAverage() { return conv_average; }

            // Scoring Accessors
            unsigned char getAverageColorScore() { return this->score_average_color; }
            unsigned char getAreaScore() { return this->score_area; }
            unsigned char getSizeScore() { return this->score_size; }
            unsigned char getConvolutionAverageScore() { return this->score_conv_average; }

            // Mutators
            void explode() { this->core.explode(); }  // Max-size exceeded. Blob exploded... ( blob_detection )
            void setCorePixels( std::vector< Pixel > core_pixels ) { this->core_pixels = core_pixels; }
            void setCore( Core core ) { this->core = core; }
            void setConvolutionSum( unsigned int conv_sum ) { this->conv_sum = conv_sum; }
            void setConvolutionAverage( unsigned char conv_average ) { this->conv_average = conv_average; }

            // Scoring Mutators
            void setAverageColorScore( unsigned char score_average_color ) { this->score_average_color = score_average_color; }
            void setAreaScore( unsigned char score_area ) { this->score_area = score_area; }
            void setSizeScore( unsigned char score_size ) { this->score_size = score_size; }
            void setConvolutionAverageScore( unsigned char score_conv_average ) { this->score_conv_average = score_conv_average; }
    };
};

