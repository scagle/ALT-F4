#include "algorithm.hpp"

#include "blob.hpp"
#include "tuner.hpp"
#include "core.hpp"
#include "datatypes/pixel.hpp"
#include "datatypes/pixel_1.hpp"

#include "globals.hpp"

#include <stack>
#include <string>
#include <csignal>

namespace altf4
{
    namespace algorithm
    {
        // Function Prototypes
        void formulateBlob( Blob& blob, std::vector< unsigned char* >& binary_data_2d, 
            std::vector< std::vector< Color > >& color_2d, 
            std::stack< Position >& edited_pixels, int row, int col, unsigned char check_value, 
            int min_neighbors, int max_size ); 
        float rigorouslyScoreBlob( Blob& blob, 
            std::vector< std::vector< Color > >& color_2d, 
            std::vector< unsigned char* >& binary_data_2d, int type, int camera_index );
        Core calculateCore( Blob& blob, std::vector< std::vector< Color > >& color_2d, 
            std::vector< unsigned char* >& binary_data_2d );

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

        // Get Convolution of the image
        void writeConvData( std::vector< std::vector< Color > >& color_2d, 
            std::vector< std::vector< unsigned char > >& conv_data, 
            std::vector< unsigned char >& conv_data_1d, 
            const std::vector< std::vector< int > >& kernel )
        {
            int rows = color_2d.size();
            int cols = color_2d[0].size();
            std::vector< int > temp(rows * cols, 0);

            conv_data_1d.resize(rows * cols);
            conv_data.resize(rows);
            for ( unsigned int row = 0; row < rows; row++ )
            {
                conv_data[row].resize(cols);
            }
            int dot_min = 0;
            int dot_max = 0;
            for ( int row = 1; row < rows - 1; row++ )
            {
                for ( int col = 1; col < cols - 1; col++ )
                {
                    // Since we're dealing with symmetric kernels, we don't need to flip both axes:
                    // http://www.songho.ca/dsp/convolution/convolution.html#convolution_2d
                    int kernel_rows = kernel.size();
                    int kernel_cols = kernel[0].size();
                    int dotproduct = 0;
                    for ( int ker_row = -1; ker_row <= 1; ker_row++ )
                    {
                        for ( int ker_col = -1; ker_col <= 1; ker_col++ )
                        {
                            dotproduct += color_2d[row - ker_row][col - ker_col].r * 
                                          kernel[ker_row + 1][ker_col + 1];
                        }
                    }
                    if ( dotproduct > dot_max )
                        dot_max = dotproduct;
                    if ( dotproduct < dot_min )
                        dot_min = dotproduct;
                    temp[((cols - 2) * (row - 1)) + (col - 1)] = dotproduct;
                }
            }

            // Normalize
            int dot_range = dot_max - dot_min;
            for ( int row = 1; row < rows - 1; row++ )
            {
                for ( int col = 1; col < cols - 1; col++ )
                {
                    int value = temp[((cols - 2) * (row - 1)) + (col - 1)];
                    unsigned char normalized_value = ( 
                            (float)( value - dot_min ) / (float)( dot_max - dot_min ) 
                        ) * 255;
                    conv_data[row][col] = normalized_value;
                    conv_data_1d[(cols * row) + col] = normalized_value;
                }
            }
        }

        // Read original image, apply pixel color thresholds, and extract binary image
        void writeBinaryData( Image* image, Image& binary_image, 
            const std::pair< Color, Color >& thresholds )
        {
            binary_image = Image( image->getRows(), image->getCols(), 1 ); 

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
                pass &= ( (*data)[i * channels + 0] >= thresholds.first.b && 
                          (*data)[i * channels + 0] <= thresholds.second.b );
                pass &= ( (*data)[i * channels + 1] >= thresholds.first.g && 
                          (*data)[i * channels + 1] <= thresholds.second.g );
                pass &= ( (*data)[i * channels + 2] >= thresholds.first.r && 
                          (*data)[i * channels + 2] <= thresholds.second.r );

                if ( pass )
                    (*binary_data)[i] = 255;
            }
        }

        void transDimensiateBinaryImage( Image& binary_image, 
            std::vector< unsigned char* >& binary_data_2d )
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

        void formulateBlob( Blob& blob, std::vector< unsigned char* >& binary_data_2d, 
            std::vector< std::vector< Color > >& color_2d, std::stack< Position >& edited_pixels, 
            int row, int col, unsigned char check_value, int min_neighbors, int max_size )
        {
            // Initialize blobbing structures
            std::vector< Pixel > blob_pixels;    // vector of all pixels in blob
            std::stack< Position > blob_stack;   // stack of pixels to check

            // Add found pixel
            blob_stack.push( {row, col} );       // push position to stack
            blob_pixels.push_back( { { row, col }, color_2d[row][col] } ); // add pixel to blob

            // Change found pixel
            binary_data_2d[row][col] = 128;      // prevent this blob from getting blobbed twice
            edited_pixels.push( { row, col } );  // record change, so we can change back later
            

            // Find other neighbor pixels that are blobbable
            int blob_min_row = row;     // keep track of boundaries of blob
            int blob_min_col = col;     // keep track of boundaries of blob
            int blob_max_row = row;     // keep track of boundaries of blob 
            int blob_max_col = col;     // keep track of boundaries of blob
            while ( !blob_stack.empty() )        // Keep checking if there's positions to check
            {
                // Grab pixel
                Position check_pos = blob_stack.top(); // grab next position off stack
                int check_row = check_pos.a;
                int check_col = check_pos.b;
                blob_stack.pop();                      // pop next position off stack        

                // Check all 8 neighbors
                unsigned int neighbor_count = 0;

                // row offset
                for ( int ro = -MAX_RELATIVE_OFFSET; ro <= MAX_RELATIVE_OFFSET; ro++ ) 
                {
                    // col offset
                    for ( int co = -MAX_RELATIVE_OFFSET; co <= MAX_RELATIVE_OFFSET; co++ ) 
                    {
                        if ( !( ro == 0 && co == 0 ) &&        // if not in center
                              ( check_row + ro <  MAX_ROW ) && // if within screen
                              ( check_row + ro >= MIN_ROW ) && // if within screen
                              ( check_col + co <  MAX_COL ) && // if within screen
                              ( check_col + co >= MIN_COL ) )  // if within screen
                        {
                            if ( binary_data_2d[check_row + ro][check_col + co] == check_value )
                            {
                                if ( max_size >= 0 && blob_pixels.size() > max_size )
                                {
                                    blob.explode();
                                    return;
                                }
                                // Add new position to stack
                                blob_stack.push( { check_row + ro, check_col + co } );     
                                blob_pixels.push_back( { { check_row + ro, check_col + co }, 
                                    color_2d[check_row + ro][check_col + co] } );

                                // prevent this blob from getting blobbed twice
                                binary_data_2d[check_row + ro][check_col + co] = 128;      
                                // record change, so we can change back later
                                edited_pixels.push( { check_row + ro, check_col + co } );  

                                // Update boundaries of blob
                                if ( check_row + ro < blob_min_row )
                                    blob_min_row = check_row + ro;
                                if ( check_col + co < blob_min_col )
                                    blob_min_col = check_col + co;
                                if ( check_row + ro > blob_max_row )
                                    blob_max_row = check_row + ro;
                                if ( check_col + co > blob_max_col )
                                    blob_max_col = check_col + co;
                            }

                            //else if (edited_matrix[p_row + ro][p_col + co] != 0)
                            //{
                            //    // make sure they're IMMEDIATE neighbors (Max of 8)
                            //    if ( ( std::abs(ro) <= 1 ) && ( std::abs(co) <= 1 ) ) 
                            //    {
                            //        neighbor_count++;
                            //    }
                            //}
                        }
                    }
                }
            }

            // Directly modify referenced blob
            blob = Blob( blob_pixels, { blob_min_row, blob_min_col, blob_max_row, blob_max_col } );
        }

        void getBlobs( Image* image, std::vector< std::vector< Color > >& color_2d, 
            std::vector< unsigned char* >& binary_data_2d, std::vector< Blob >& all_blobs )
        {
            unsigned int rows = image->getRows();
            unsigned int cols = image->getCols();

            std::stack< Position > edited_pixels; // stack of modified pixels, to be reverted later
            unsigned int blob_count = 0;

            for (int row = 0; row < (int)rows; row++)
            {
                for (int col = 0; col < (int)cols; col++)
                {
                    // If binary_pixel is true/passed/white
                    if ( binary_data_2d[row][col] == 255 )
                    {
                        all_blobs.push_back( Blob() );
                        Blob& blob = all_blobs[all_blobs.size() - 1];
                        formulateBlob( blob, binary_data_2d, color_2d, 
                            edited_pixels, row, col, 255, 8, -1 );

                        blob_count++;
                    }
                }
            }
            //printf("Found %d blobs!\n", blob_count);

            // Reset changed pixels back to what they were
            while ( !edited_pixels.empty() )
            {
                Position pos = edited_pixels.top();
                binary_data_2d[pos.a][pos.b] = 255;
                edited_pixels.pop();
            }
        }

        unsigned char scoreAverageColor( Color& average_color, const Color& expected_color, 
            int multiplier )
        {
            // Get distance in pixels
            unsigned int diff_c1 = std::abs( (int)average_color.b - (int)expected_color.b );
            unsigned int diff_c2 = std::abs( (int)average_color.g - (int)expected_color.g );
            unsigned int diff_c3 = std::abs( (int)average_color.r - (int)expected_color.r );

            // Normalize (0 - 765) to (0 - 255)
            unsigned int normalized_diff = ( 
                (float)( diff_c1 + diff_c2 + diff_c3 ) / ( 255.0 * 3.0 ) * 255.0 
            );

            // Invert, so that the lower number gives the higher score 
            return ( 255 - normalized_diff );
        }

        unsigned char scoreArea( unsigned int area, const unsigned int expected_area, 
            int multiplier )
        {
            unsigned int diff_area = (int)expected_area - (int)area;

            int normalized_diff = 255 - diff_area * multiplier;
            if ( normalized_diff > 255 )
                normalized_diff = 255;
            if ( normalized_diff < 0 )
                normalized_diff = 0;

            return (unsigned char)normalized_diff;
        }

        unsigned char scoreSize( unsigned int size, const unsigned int expected_size, 
            int multiplier )
        {
            unsigned int diff_size = (int)expected_size - (int)size;

            int normalized_diff = 255 - diff_size * multiplier;
            if ( normalized_diff > 255 )
                normalized_diff = 255;
            if ( normalized_diff < 0 )
                normalized_diff = 0;

            return (unsigned char)normalized_diff;
        }

        unsigned char scoreAverageCoreColor( const Color average_color, const Color expected_color, 
            const std::vector< bool > channel_masks, int multiplier )
        {
            // Get distance in pixels
            unsigned int diff_c1 = std::abs( (int)average_color.b - (int)expected_color.b );
            unsigned int diff_c2 = std::abs( (int)average_color.g - (int)expected_color.g );
            unsigned int diff_c3 = std::abs( (int)average_color.r - (int)expected_color.r );

            unsigned int total_score = 0;
            unsigned int num_scores = 0;
            if ( channel_masks[0] )
            {
                total_score += 255 - (float)( diff_c1 );
                num_scores++;
            }
            if ( channel_masks[1] )
            {
                total_score += 255 - (float)( diff_c2 );
                num_scores++;
            }
            if ( channel_masks[2] )
            {
                total_score += 255 - (float)( diff_c3 );
                num_scores++;
            }

            // Invert, so that the lower number gives the higher score 
            return total_score / num_scores;
        }

        unsigned char scoreAverageCoreLength( const float average_core_length, 
            const int expected_length, int multiplier )
        {
            float diff_length = expected_length - average_core_length;

            int normalized_diff = 255 - diff_length * multiplier;
            if ( normalized_diff > 255 )
                normalized_diff = 255;
            if ( normalized_diff < 0 )
                normalized_diff = 0;

            return (unsigned char)normalized_diff;
        }

        unsigned char scoreConvolutionAverage( unsigned char average, 
            const unsigned int expected_average, int multiplier )
        {
            unsigned int diff_average = (int)expected_average - (int)average;

            int normalized_diff = 255 - diff_average * multiplier;
            if ( normalized_diff > 255 )
                normalized_diff = 255;
            if ( normalized_diff < 0 )
                normalized_diff = 0;

            return (unsigned char)normalized_diff;
        }

        void scoreBlobs( std::vector< std::vector< Color > >& color_2d, 
            std::vector< unsigned char* >& binary_data_2d, 
            std::vector< Blob >& blobs, Blob& best_blob, int type, int camera_index )
        {
            int blob_count = 0;
            float best_score = 0;

            for ( auto&& blob : blobs )
            {
                Color color = blob.getAverageColor();
                unsigned int area = blob.getArea();
                unsigned int size = blob.getSize();

                unsigned char score_average_color = 0;
                unsigned char score_area = 0;
                unsigned char score_size = 0;

                float score_sum = 0;
                float score_max = 0;
                
                if ( Tuner::scoring_masks[0] )
                {
                    score_average_color = scoreAverageColor( color, 
                        Tuner::hsv_expected_values[camera_index][type], 1 );
                    score_sum += score_average_color;
                    score_max += 255;
                }

                if ( Tuner::scoring_masks[1] )
                {
                    score_area = scoreArea( area, Tuner::expected_areas[type], 2 );
                    score_sum += score_area;
                    score_max += 255;
                }

                if ( Tuner::scoring_masks[2] )
                {
                    score_size = scoreSize( size, Tuner::expected_sizes[type], 1 );
                    score_sum += score_size;
                    score_max += 255;
                }

                float percent_score = ( score_sum / score_max ); // Get percentage score
                if ( percent_score >= Tuner::percentage_score_cutoff[camera_index] )
                {
                    // Save scores, since they matter more now ( and so we can view them later
                    blob.addAttribute( "score_average_color", score_average_color, 
                        color.getStructuredString() );
                    blob.addAttribute( "score_area", score_area, std::to_string( area ) );
                    blob.addAttribute( "score_size", score_size, std::to_string( size ) );
                    blob.addAttribute( "percent_score", percent_score, 
                        std::to_string( percent_score ) );

                    // Apply more rigorous testing on the higher scoring blobs to save resources
                    float multiplier = rigorouslyScoreBlob( blob, color_2d, binary_data_2d, 
                        type, camera_index );
                    percent_score *= multiplier;

                    if ( percent_score > best_score )
                    {
                        best_score = percent_score;
                        best_blob = blob;
                    }
                }
                blob_count++;
            }
        }

        float rigorouslyScoreBlob( Blob& blob, std::vector< std::vector< Color > >& color_2d, 
                std::vector< unsigned char* >& binary_data_2d, int type, int camera_index )
        {
            // Do intensive things that only the best of blobs need here:
            // Get Convolution Score
            convoluteBlob( blob, color_2d, Tuner::convolution_kernel );
            // Find core
            blob.setCore( calculateCore( blob, color_2d, binary_data_2d ) );
            Core* core = blob.getCore();
            core->spread( color_2d, binary_data_2d );

            // Calculate score multiplier
            float multiplier = 1;

            // Core expected average color
            if ( Tuner::scoring_rigorous_masks[0] )
            {
                Color& average_color = core->getAverageColor();
                unsigned char score_average_core_color = scoreAverageCoreColor( average_color, 
                    Tuner::expected_core_colors[type], Tuner::core_colors_masks, 2 );
                multiplier *= (float)score_average_core_color;
                blob.addAttribute( "average_core_color", score_average_core_color, 
                    average_color.getStructuredString() );
            }

            // Core expected average length
            if ( Tuner::scoring_rigorous_masks[1] )
            {
                float average_core_length = core->getAverageLength();
                unsigned char score_average_core_length = scoreAverageCoreLength( 
                    average_core_length, Tuner::expected_core_length[type], 50 
                );
                multiplier *= (float)score_average_core_length;
                blob.addAttribute( "average_core_length", score_average_core_length, 
                    std::to_string( average_core_length ) );
            }

            // Core exploded
            if ( Tuner::scoring_rigorous_masks[2] )
            {
                bool exploded = blob.isExploded();
                multiplier *= (exploded) ? 0.5 : 1;
                blob.addAttribute( "exploded", (float)exploded, 
                    std::to_string( (exploded) ? 0.5 : 1 ) );
            }

            // Convolution Average
            if ( Tuner::scoring_rigorous_masks[3] )
            {
                unsigned char conv_average = blob.getConvolutionAverage();
                unsigned char score_conv_average = scoreConvolutionAverage( conv_average, 
                    Tuner::expected_conv_averages[type], 1 );
                float normalized_score = (float)score_conv_average / 255.0;
                multiplier *= normalized_score;
                blob.addAttribute( "score_conv_average", normalized_score, 
                    std::to_string( conv_average ) );
            }


            //multiplier *= blob.getNormalizedEccentricity();
            // TODO: Broken and not well implemented
            //multiplier *= blob.getNormalizedCoreArea();

            return multiplier;
        }

        void convoluteBlob( Blob& blob, std::vector< std::vector< Color > >& color_2d, 
                const std::vector< std::vector< int > >& kernel )
        {
            std::vector< Pixel >* blob_pixels = blob.getPixels();       // Blob pixels (3 channels)

            // Normalized Convolution pixels (1 channel) ( 0 - 255 )
            std::vector< Pixel_1 >* conv_pixels = blob.getConvPixels(); 
            std::vector< int > product_values;   // Convolution before normalization ( + / - )

            int dot_product_min = 0;
            int dot_product_max = 0;

            for ( Pixel& pixel : *blob_pixels )
            {
                const int row = pixel.position.a;
                const int col = pixel.position.b;

                int dot_product_sum = 0;

                for ( int kernel_row = -1; kernel_row <= 1; kernel_row++ )
                {
                    for ( int kernel_col = -1; kernel_col <= 1; kernel_col++ )
                    {
                        // Check edge cases
                        if ( ( row - kernel_row >= 0 )      && 
                             ( row - kernel_row < MAX_ROW ) && 
                             ( col - kernel_col >= 0 )      &&
                             ( col - kernel_col < MAX_COL ) )
                        { 
                            dot_product_sum += color_2d[row - kernel_row][col - kernel_col].r 
                                * kernel[kernel_row + 1][kernel_col + 1];
                        }
                    }
                }

                if ( dot_product_sum > dot_product_min )
                    dot_product_min = dot_product_sum;
                if ( dot_product_sum < dot_product_max )
                    dot_product_max = dot_product_sum;

                product_values.push_back( dot_product_sum );
            }

            if ( (*blob_pixels).size() != product_values.size() )
            {
                printf("*** WARNING: blob_pixels size: %d | product_values size: %d\n", 
                    (*blob_pixels).size(), product_values.size());
            }
            // Normalize
            int total_dot_product_sum = 0;
            int dot_product_range = dot_product_max - dot_product_min;
            int i = 0;
            for ( auto&& conv_value : product_values )
            {
                int row = (*blob_pixels)[i].position.a;
                int col = (*blob_pixels)[i].position.b; 
                unsigned char normalized_value = ( (float)( conv_value - dot_product_min ) 
                    / (float)( dot_product_max - dot_product_min ) ) * 255;
                conv_pixels->push_back( { { row, col }, normalized_value } );
                total_dot_product_sum += normalized_value;
                i++;
            }

            // Set total convolution sum
            blob.setConvolutionSum( total_dot_product_sum );
            // Set ratio convolution sum
            blob.setConvolutionAverage( total_dot_product_sum / (float)(product_values.size()) );
        }

        Core calculateCore( Blob& blob, 
                std::vector< std::vector< Color > >& color_2d, 
                std::vector< unsigned char* >& binary_data_2d )
        {
            std::vector< Pixel >* blob_pixels = blob.getPixels(); // Blob pixels
            cv::Rect general_box = blob.getEncompassingRect(0);   // Get general box

            // Occupy General Box
            std::vector< Pixel > potentially_bright_pixels; // General array that (hopefully) 
                                                            // encompasses laser core     
            for ( int row = 0; row < general_box.height; row++ )
            {
                for ( int col = 0; col < general_box.width; col++ )
                {
                    if ( ( general_box.y + row <  MAX_ROW ) && // if within screen
                         ( general_box.y + row >= MIN_ROW ) && // if within screen
                         ( general_box.x + col <  MAX_COL ) && // if within screen
                         ( general_box.x + col >= MIN_COL ) )  // if within screen
                    {
                        potentially_bright_pixels.push_back( 
                            Pixel{
                                {
                                    general_box.y + row,
                                    general_box.x + col, 
                                    ROW_COL
                                },
                                color_2d[general_box.y + row][general_box.x + col] 
                            }
                        );
                    }
                }
            }

            // Find K brightest pixels
            std::vector< Pixel > brightest_pixels; // K brightest pixels
            for (int search = 0; search < Tuner::number_of_whitest_pixel_searches; search++ )
            {
                unsigned int max_brightness = 0;
                Pixel* brightest_pixel = nullptr;
                int brightest_index = 0;
                int index = 0;
                for ( auto&& pixel : potentially_bright_pixels )
                {
                    unsigned int brightness = ( pixel.color.b + pixel.color.g + pixel.color.r );
                    if ( brightness > max_brightness )
                    {
                        max_brightness = brightness;
                        brightest_pixel = &pixel;
                        brightest_index = index;
                    }
                    index++;
                }

                if ( brightest_pixel == nullptr )
                    break;
                brightest_pixels.push_back( *brightest_pixel );
                brightest_pixel->color = {0, 0, 0}; // We don't want to record the same one twice
            }

            // Average K brightest pixels
            int average_row = 0;
            int average_col = 0;
            for ( auto&& bright_pixel : brightest_pixels )
            {
                average_row += bright_pixel.position.a;
                average_col += bright_pixel.position.b;
            }
            average_row /= brightest_pixels.size();
            average_col /= brightest_pixels.size();
            //printf ("Average Pixel = %d, %d\n", average_row, average_col);

            Position core_origin = {average_row, average_col, PositionType::ROW_COL};

            return Core( core_origin, blob.getBoundary() );
        }


    };
};

