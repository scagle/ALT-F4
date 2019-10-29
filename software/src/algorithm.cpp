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

        void transDimensiateBinaryMatrix( Image& binary_image, std::vector< unsigned char* >& binary_data_2d )
        {
            unsigned int rows = binary_image.getRows();
            unsigned int cols = binary_image.getCols();
            binary_data_2d.resize(cols, nullptr);

            for ( unsigned int i = 0; i < rows; i++ )
            {
                binary_data_2d[i] = &((*binary_image.getData())[i * cols]);
            }

            //for ( unsigned int row = 0; row < rows; row++ )
            //{
            //    for ( unsigned int col = 0; col < cols; col++ )
            //    {
            //        printf("%d ", binary_data_2d[row][col]);
            //    }
            //    printf("\n(%d, X) rows: %d, cols: %d\n", row, rows, cols);
            //}
            //printf("DONE\n");
        }
    };
};

