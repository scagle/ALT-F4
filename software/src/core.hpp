#pragma once

#include "datatypes/position.hpp"
#include "datatypes/core_anchor.hpp"
#include "datatypes/boundary.hpp"
#include "datatypes/pixel.hpp"

#include <opencv2/videoio.hpp>
#include <vector>

namespace altf4
{
    class Core
    {
        private:
            // Private Members
            bool initialized = false;
            Position origin;
            Color avg_color;
            float avg_length;
            std::vector< CoreAnchor > anchors;
            Boundary* blob_boundary;
            Boundary core_boundary;
            bool exploded = false;

        public:
            Core() { }
            Core( Position origin, Boundary* blob_boundary ) : initialized(true), origin(origin), blob_boundary(blob_boundary) { }
            virtual ~Core() { }

            // Methods
            CoreAnchor getAnchor( std::vector< unsigned char* >& binary_data_2d, std::vector< std::vector< Color > >& color_2d, 
                    unsigned int num_rows, unsigned int num_columns, int dir_row, int dir_col);
            void spread( std::vector< std::vector< Color > >& color_2d, std::vector< unsigned char* >& binary_data_2d );
            cv::Rect getEncompassingRect( int padding );
            unsigned int getArea();
            Position getOrigin() { return this->origin; }
            unsigned int getEccentricity(); // Calculate eccentricity

            // Accessors
            bool isInitialized() { return this->initialized; }
            std::vector< CoreAnchor >& getAnchors() { return this->anchors; }
            Color& getAverageColor() { return this->avg_color; }
            float getAverageLength() { return this->avg_length; }
            bool isExploded() { return this->exploded; }

            // Mutators
            void explode() { this->exploded = true; }
    };
};

