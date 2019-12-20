#include "renderer.hpp"

#include "core.hpp"
#include "tuner.hpp"

#include "datatypes/position.hpp"
#include "datatypes/core_anchor.hpp"

#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

#include <csignal>
#include <vector>
#include <string>
#include <set>
#include <fstream>

template class std::vector<Attribute>;

namespace altf4
{
    // Static Declarations

    // Constructors

    // Methods
    void Renderer::renderMats( std::vector< DataFrame >* frames, 
        std::vector< cv::Mat3b >* original_images )
    {
        if ( pause )
        {
            // OpenCV needs waitKey to handle input ( otherwise it will freeze forever )
            window.wait(10); 
            return;
        }
        for ( unsigned int i = 0; i < frames->size(); i++ )
        {
            // REFACTOR TODO: Do checks to see if dataframe is really initialized
            Image* image = (*frames)[i].getImage();
            std::vector< unsigned char >& conv_data_1d = (*frames)[i].getConvData1D();
            std::vector< Image >& binary_images = (*frames)[i].getBinaryImages();
            
            if ( image == nullptr || image->isEmpty() )
                continue;

            int rows = image->getRows();
            int cols = image->getCols();

            mats.push_back(cv::Mat(rows, cols, CV_8UC3, image->getData()->data()));
            //conv_mats.push_back(cv::Mat(rows, cols, CV_8UC1, conv_data_1d.data()));
            all_binary_mats[i].clear();
            for ( unsigned int type = 0; type < binary_images.size(); type++ )
            {
                all_binary_mats[i].push_back(cv::Mat(rows, cols, CV_8UC1, 
                    binary_images[type].getData()->data()));
            }

            if (display_type == 0)
            {
                drawBlobBoundaries( &( (*original_images)[i] ), (*frames)[i].getBestBlobs() );
                drawAttributes( &( (*original_images)[i] ), (*frames)[i].getBestBlobs() );
                window.render( i, &( (*original_images)[i] ) );
            }
            else if (display_type == 1)
            {
                drawBlobBoundaries( &( mats[i] ), (*frames)[i].getBestBlobs() );
                drawAttributes( &( mats[i] ), (*frames)[i].getBestBlobs() );
                window.render( i, &( mats[i] ) );
            }
            else if (display_type == 2)
            {
                // Draw Detailed Blob / Core picture ( debugging )
                blobAndCoreToImage( i, (*frames)[i].getBinaryDatas2D()[0], 
                    (*frames)[i].getBestBlobs()[0] );
                writeAttributesToFile( &( mats[i] ), (*frames)[i].getBestBlobs() );

                //drawBlobBoundaries( &( conv_mats[i] ), (*frames)[i].getBestBlobs() );
                ////drawConvolutionSpots( i, (*frames)[i].getBestBlobs() );
                //window.render( i, &( conv_mats[i] ) );
            }
            else
            {
                if ( all_binary_mats.size() > 0 && 
                     display_type - 3 < (int)all_binary_mats[0].size()  )
                {
                    drawBlobBoundaries( &( all_binary_mats[i][display_type-3] ), 
                        (*frames)[i].getBestBlobs() );
                    window.render( i, &( all_binary_mats[i][display_type-3] ) );
                }
                else
                {
                    printf("Invalid display_type '%d'\n", display_type);
                    display_type = 0;
                }
            }
        }
    }
    
    void Renderer::drawBlobBoundaries( cv::Mat* mat, std::vector< Blob >& best_blobs )
    {
        for ( unsigned int type = 0; type < best_blobs.size(); type++ )
        {
            if ( best_blobs[type].isInitialized() )
            {
                cv::rectangle( *mat, best_blobs[type].getEncompassingRect(10), 
                    Tuner::associated_color[type], 2 );
            }
        }
    }

    void Renderer::writeAttributesToFile( cv::Mat* mat, std::vector< Blob >& best_blobs )
    {
        // Check if out file is open, if not try opening it
        if ( this->out_file.is_open() == false ) 
        {
            this->out_file.open("attributes.csv");
            if ( this->out_file.is_open() == false )
            {
                printf("*** WARNING: Can't open attribute output file (renderer.cpp)\n");
                return;
            }
            printf("Successfully opened attributes file");
        }

        std::set< std::string > shown_attributes = 
        {
            "percent_score", 
            "score_average_color",
            "score_area", 
            "score_size", 
            "average_core_color",
            "average_core_length",
            "exploded",
            "score_conv_average",
        };

        // Grab red blobs
        Blob& blob = best_blobs[0]; 

        // Output attribute names
        if ( out_file_started == false )
        {
            if ( blob.isInitialized() )
            {
                std::vector< Attribute >& attributes = blob.getAttributes();
                std::ostringstream ss;
                for ( unsigned int i = 0; i < attributes.size(); i++ )
                {
                    if ( shown_attributes.find(attributes[i].name) != shown_attributes.end() )
                    {
                        ss << attributes[i].name;
                        printf("%s\n", attributes[i].name.c_str());
                        if ( i != attributes.size() - 1 )
                        {
                            ss << ",";
                        }
                    }
                }
                out_file << ss.str() << "\n";
                out_file_started = true;
            }
        }

        // Output attribute values
        if ( blob.isInitialized() )
        {
            std::vector< Attribute >& attributes = blob.getAttributes();
            std::ostringstream ss;
            for ( unsigned int i = 0; i < attributes.size(); i++ )
            {
                if ( shown_attributes.find(attributes[i].name) != shown_attributes.end() )
                {
                    ss << attributes[i].score;
                    printf("%f\n", attributes[i].score);
                    if ( i != attributes.size() - 1 )
                    {
                        ss << ",";
                    }
                }
            }
            out_file << ss.str() << "\n";
        }
    }

    void Renderer::drawAttributes( cv::Mat* mat, std::vector< Blob >& best_blobs )
    {
        std::set< std::string > shown_attributes = 
        {
            //"percent_score", 
            "score_average_color",
            "score_area", 
            "score_size", 
            //"average_core_color",
            "average_core_length",
            //"exploded",
            "score_conv_average",
        };
        int type = 0;
        for ( auto&& blob : best_blobs )
        {
            if ( blob.isInitialized() )
            {
                std::vector< Attribute >& attributes = blob.getAttributes();
                std::ostringstream ss;
                for ( unsigned int i = 0; i < attributes.size(); i++ )
                {
                    if ( shown_attributes.find(attributes[i].name) != shown_attributes.end() )
                    {
                        switch ( attribute_type )
                        {
                            case 0:
                                ss << attributes[i].score;
                                break;
                            case 1:
                                ss << attributes[i].text;
                                break;
                            case 2:
                                ss << attributes[i].name;
                                break;
                            default:
                                break;
                        }
                        if ( i != attributes.size() - 1 )
                        {
                            ss << ", ";
                        }
                    }
                }
                cv::putText( *mat, ss.str(), cv::Point(10, 470 - (type * 20)), 
                             cv::FONT_HERSHEY_SIMPLEX, 0.5, Tuner::associated_color[type], 2);
            }
            type++;
        }
    }

    //void Renderer::drawConvolutionSpots( int index, std::vector< Blob >& best_blobs )
    //{
    //    for ( unsigned int type = 0; type < best_blobs.size(); type++ )
    //    {
    //        if ( best_blobs[type].isInitialized() )
    //        {

    //        }
    //    }
    //}

    bool Renderer::initialize( int number_of_cameras )
    {
        all_binary_mats.resize( number_of_cameras );
        window.initialize( number_of_cameras );
        return true;
    }

    void Renderer::setDisplayType( unsigned char display_type ) 
    { 
        this->display_type = display_type; 
        window.updateTune( display_type );
    }

    int absoluteToRelative( int absolute, int relative_start, int relative_end )
    {
        int relative = absolute - relative_start;
        if ( relative < 0 )
        {
            return -1;
        }
        if ( relative > relative_end - relative_start - 1 )
        {
            return -1;
        }
        return relative;
    }
    cv::Point positionToPoint( Position pos )
    {
        if ( pos.t == ROW_COL )
            return cv::Point( pos.b, pos.a );
        else
            return cv::Point( pos.a, pos.b );
    }

    void Renderer::blobAndCoreToImage( int window_index, 
        std::vector< unsigned char* >& binary_data_2d, Blob& blob )
    {
        if (!blob.isInitialized())
        {
            printf("NO GREEN BLOB!\n");
            window.wait(10);
            return;
        }
        Core* core = blob.getCore();
        Position core_origin = core->getOrigin();
        cv::Rect blob_rect = blob.getEncompassingRect(0);

        cv::Mat image( blob_rect.height, blob_rect.width, CV_8UC3, cv::Scalar(0, 0, 0) );
        const int start_row = blob_rect.y;
        const int end_row = blob_rect.y + blob_rect.height;
        const int start_col = blob_rect.x;
        const int end_col = blob_rect.x + blob_rect.width;

        for ( int row = start_row; row < end_row; row++ )
        {
            for ( int col = start_col; col < end_col; col++ )
            {
                const int rel_row = absoluteToRelative(row, start_row, end_row);
                const int rel_col = absoluteToRelative(col, start_col, end_col);

                if ( binary_data_2d[row][col] == 255 )
                {
                    image.at< cv::Vec3b >( cv::Point( rel_col, rel_row ) )[0] = 100;
                    image.at< cv::Vec3b >( cv::Point( rel_col, rel_row ) )[1] = 100;
                    image.at< cv::Vec3b >( cv::Point( rel_col, rel_row ) )[2] = 100;
                }
            }
        }

        const int rel_row_origin = absoluteToRelative(core_origin.a, start_row, end_row);
        const int rel_col_origin = absoluteToRelative(core_origin.b, start_col, end_col);
        image.at< cv::Vec3b >( cv::Point( rel_col_origin, rel_row_origin ) )[2] = 255;

        std::vector< CoreAnchor >& anchors = core->getAnchors();
        for ( auto&& anchor : anchors )
        {
            const int rel_row_anchor = absoluteToRelative(anchor.anchor.a, start_row, end_row);
            const int rel_col_anchor = absoluteToRelative(anchor.anchor.b, start_col, end_col);
            //printf("[0 - %d] get %d\n", end_row - start_row, rel_row_anchor);
            //printf("[0 - %d] get %d\n", end_col - start_col, rel_col_anchor);

            // Skip if -1
            if ( rel_row_anchor < 0 || rel_col_anchor < 0 )
                continue;

            image.at< cv::Vec3b >( cv::Point( rel_col_anchor, rel_row_anchor ) )[0] = 255;
            //image.at< cv::Vec3b >( cv::Point( rel_col_anchor, rel_row_anchor ) )[1] = 255;
            //image.at< cv::Vec3b >( cv::Point( 0, 0 ) )[0] = 255;
            //image.at< cv::Vec3b >( cv::Point( 0, 0 ) )[1] = 255;

        }

        window.render( window_index, &( image ) );
    }
};

