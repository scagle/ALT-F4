#pragma once

#include "datatypes/position.hpp"
#include "datatypes/pixel.hpp"

#include <opencv2/videoio.hpp>
#include <vector>

namespace altf4
{
    class Core
    {
        public:
            // Public Enums

        private:
            // Private Members
            Position origin;
            std::vector< Position > anchors;
            

        public:
            Core() { }
            Core( Position origin ) : origin(origin) { }
            virtual ~Core() { }

            // Methods
            Position getAnchor( std::vector< unsigned char* >& binary_data_2d, int dir_row, int dir_col);
            void spread( std::vector< std::vector< Color > >& color_2d, std::vector< unsigned char* >& binary_data_2d );
            cv::Rect getEncompassingRect( int padding );
            unsigned int getArea();
            unsigned int getEccentricity(); // Calculate eccentricity

            // Accessors
            std::vector< Position >& getAnchors() { return this->anchors; };

            // Mutators
    };
};

