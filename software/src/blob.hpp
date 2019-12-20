#pragma once

#include "core.hpp"

#include "datatypes/pixel.hpp"
#include "datatypes/pixel_1.hpp"
#include "datatypes/color.hpp"
#include "datatypes/attribute.hpp"

#include <vector>
#include <unordered_map>

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
            std::vector< Attribute > attributes; 
            Core core;
            Boundary boundary; // Boundary of blob
            bool initialized = false; // If blob is created but not initialized

            // Rigorous variable storage
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
            std::vector< Attribute >& getAttributes() { return this->attributes; }

            // Mutators
            void explode() { this->core.explode(); }  // Max-size exceeded. 
            void setCorePixels( std::vector< Pixel > core_pixels ) 
                { this->core_pixels = core_pixels; }
            void setCore( Core core ) { this->core = core; }
            void addAttribute( std::string name, float score, std::string text ) 
                { this->attributes.push_back( { name, score, text } ); }

            // Rigorous Accessors/Mutators
            void setConvolutionSum( unsigned int conv_sum ) { this->conv_sum = conv_sum; }
            void setConvolutionAverage( unsigned char conv_average ) 
                { this->conv_average = conv_average; }
            unsigned int getConvolutionSum() { return conv_sum; }
            unsigned char getConvolutionAverage() { return conv_average; }

    };
};

