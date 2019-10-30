#include "algorithm.hpp"

#include "globals.hpp"
#include "datatypes/pixel.hpp"
#include <stack>

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

        void transDimensiateImage( Image* image, std::vector< std::vector< Color > >& color_2d )
        {
            unsigned int rows = image->getRows();
            unsigned int cols = image->getCols();
            std::vector< unsigned char >* data = image->getData();

            color_2d.resize(rows);
            for ( unsigned int row = 0; row < rows; row++ )
                color_2d[row].resize(cols);

            int count = 0;
            for ( unsigned int row = 0; row < rows; row++ )
            {
                for ( unsigned int col = 0; col < cols; col++ )
                {
                    color_2d[row][col] = { (*data)[count++], (*data)[count++], (*data)[count++] };
                }
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

        void transDimensiateBinaryImage( Image& binary_image, std::vector< unsigned char* >& binary_data_2d )
        {
            unsigned int rows = binary_image.getRows();
            unsigned int cols = binary_image.getCols();
            std::vector< unsigned char >* binary_data = binary_image.getData();
            binary_data_2d.resize(cols, nullptr);

            for ( unsigned int i = 0; i < rows; i++ )
            {
                binary_data_2d[i] = &((*binary_data)[i * cols]);
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


        void getBlobs( Image* image, std::vector< std::vector< Color > >& color_2d, 
                       std::vector< unsigned char* >& binary_data_2d, std::vector< Blob >& all_blobs )
        {
            unsigned int rows = image->getRows();
            unsigned int cols = image->getCols();

            std::stack< Position > edited_pixels; // stack of modified pixels, that allows us to change them back
            unsigned int blob_count = 0;

            for (unsigned int row = 0; row < rows; row++)
            {
                for (unsigned int col = 0; col < cols; col++)
                {
                    // If binary_pixel is true/passed/white
                    if ( binary_data_2d[row][col] == 255 )
                    {
                        // Initialize blobbing structures
                        std::vector< Pixel > blob_pixels;    // vector of all pixels in blob
                        std::stack< Position > blob_stack;   // stack of pixels to check

                        // Add found pixel
                        blob_stack.push( {row, col} );       // push position to stack
                        blob_pixels.push_back( { { row, col }, color_2d[row][col] } ); // add pixel to collection

                        // Change found pixel
                        binary_data_2d[row][col] = 0;        // prevent this blob from getting blobbed twice
                        edited_pixels.push( { row, col } );  // record change, so we can change back later
                        

                        // Find other neighbor pixels that are blobbable
                        unsigned int blob_min_row = row;     // keep track of boundaries of blob
                        unsigned int blob_min_col = col;     // keep track of boundaries of blob
                        unsigned int blob_max_row = row;     // keep track of boundaries of blob 
                        unsigned int blob_max_col = col;     // keep track of boundaries of blob
                        while ( !blob_stack.empty() )        // Keep checking if there's positions to check
                        {
                            // Grab pixel
                            Position check_pos = blob_stack.top(); // grab next position off stack
                            int check_row = check_pos.x;
                            int check_col = check_pos.y;
                            blob_stack.pop();                      // pop next position off stack        
                            //MAX_RELATIVE_OFFSET
                            //EDGE_NEIGHBOR_CUTOFF

                            // Check all 8 neighbors
                            unsigned int neighbor_count = 0;
                            for ( unsigned int ro = -MAX_RELATIVE_OFFSET; ro <= MAX_RELATIVE_OFFSET; ro++ ) // row offset
                            {
                                for ( unsigned int co = -MAX_RELATIVE_OFFSET; co <= MAX_RELATIVE_OFFSET; co++ ) // col offset
                                {
                                    if ( !( ro == 0 && co == 0 ) &&        // if not in center
                                          ( check_row + ro <  MAX_ROW ) && // if within screen
                                          ( check_row + ro >= MIN_ROW ) && // if within screen
                                          ( check_col + co <  MAX_COL ) && // if within screen
                                          ( check_col + co >= MIN_COL ) )  // if within screen
                                    {
                                        if ( binary_data_2d[check_row + ro][check_col + co] == 255 )
                                        {
                                            blob_stack.push( { check_row + ro, check_col + co } );     // Add new position to stack
                                            blob_pixels.push_back( { { check_row + ro, check_col + co }, color_2d[check_row + ro][check_col + co] } );

                                            binary_data_2d[check_row + ro][check_col + co] = 0;        // prevent this blob from getting blobbed twice
                                            edited_pixels.push( { check_row + ro, check_col + co } );  // record change, so we can change back later

                                            // Update boundaries of blob
                                            if ( check_row + ro > blob_max_row )
                                                blob_max_row = check_row + ro;
                                            if ( check_row + ro < blob_min_row )
                                                blob_min_row = check_row + ro;
                                            if ( check_col + co > blob_max_col )
                                                blob_max_col = check_col + co;
                                            if ( check_col + co < blob_min_col )
                                                blob_min_col = check_col + co;
                                        }
                                        //else if (edited_matrix[p_row + ro][p_col + co] != 0)
                                        //{
                                        //    if ( ( std::abs(ro) <= 1 ) && ( std::abs(co) <= 1 ) ) // make sure they're IMMEDIATE neighbors (Max of 8)
                                        //    {
                                        //        neighbor_count++;
                                        //    }
                                        //}
                                    }
                                }
                            }
                        }
                        all_blobs.push_back( Blob( blob_pixels, blob_min_row, blob_min_col, blob_max_row, blob_max_col ) );
                        //all_blobs.push_back( Blob( blob_pixels, edge_pixels, core_pixels, blob_min_row, blob_min_col, blob_max_row, blob_max_col ) );
                        blob_count++;
                    }
                }
            }
            printf("Found %d blobs!\n", blob_count);
            while ( !edited_pixels.empty() )
            {
                Position pos = edited_pixels.top();
                binary_data_2d[pos.x][pos.y] = 255;
                edited_pixels.pop();
            }
        }
    };
};

