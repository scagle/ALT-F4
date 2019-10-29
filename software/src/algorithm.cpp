#include "algorithm.hpp"

namespace altf4
{
    namespace algorithm
    {
        // Methods
        
        // Read original image, apply pixel color thresholds, and extract binary image
        void writeBinaryData( Image* image, Image& binary_image, const std::pair< Color, Color >& thresholds )
        {
            binary_image = Image( image->getRows(), image->getCols(), 1 ); // initialize binary_image

            std::vector< unsigned char >* data = image->getData();
            std::vector< unsigned char >* binary_data = binary_image.getData();
            
            unsigned int rows = image->getRows();
            unsigned int cols = image->getCols();
            unsigned int channels = image->getChannels();
            unsigned int data_size = rows * cols * channels;
            unsigned int binary_data_size = rows * cols * 1;

            binary_data->resize( binary_data_size, 0 );

            for ( unsigned int i = 0; i < binary_data_size; i++ )
            {
                bool pass = true;
                pass &= ( (*data)[i * channels + 0] >= thresholds.first.b && (*data)[i * channels + 0] <= thresholds.second.b );
                pass &= ( (*data)[i * channels + 1] >= thresholds.first.g && (*data)[i * channels + 1] <= thresholds.second.g );
                pass &= ( (*data)[i * channels + 2] >= thresholds.first.r && (*data)[i * channels + 2] <= thresholds.second.r );

                if ( pass )
                    (*binary_data)[i] = 255;
            }
        }

        void transDimensiate( Image* image, Image& binary_image )
        {
            printf("Transdimensiate!\n");
        }
    };
};

