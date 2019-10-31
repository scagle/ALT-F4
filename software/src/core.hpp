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
            std::vector< std::vector< Position > > tendrils;
            

        public:
            Core() { }
            Core( Position origin ) : origin(origin) { }
            virtual ~Core() { }

            // Methods
            void spread( std::vector< std::vector< Color > >& color_2d, std::vector< unsigned char* >& binary_data_2d );

            cv::Rect getEncompassingRect( int padding );

            // Accessors

            // Mutators
    };
};

