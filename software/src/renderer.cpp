#include "renderer.hpp"

#include "core.hpp"
#include "tuner.hpp"

#include "datatypes/position.hpp"
#include "datatypes/core_anchor.hpp"

#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <csignal>

namespace altf4
{
    // Static Declarations

    // Constructors

    // Methods
    void Renderer::renderMats( std::vector< DataFrame >* frames, std::vector< cv::Mat3b >* original_images )
    {
        if ( pause )
        {
            window.wait(10); // OpenCV needs waitKey to handle input ( otherwise it will freeze forever )
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
                all_binary_mats[i].push_back(cv::Mat(rows, cols, CV_8UC1, binary_images[type].getData()->data()));
            }

            if (display_type == 0)
            {
                annotateMat( i, &( (*original_images)[i] ), (*frames)[i].getBestBlobs() );
                window.render( i, &( (*original_images)[i] ) );
            }
            else if (display_type == 1)
            {
                annotateMat( i, &( mats[i] ), (*frames)[i].getBestBlobs() );
                window.render( i, &( mats[i] ) );
            }
            else if (display_type == 2)
            {
                // Draw Detailed Blob / Core picture ( debugging )
                blobAndCoreToImage( i, (*frames)[i].getBinaryDatas2D()[0], (*frames)[i].getBestBlobs()[0] );

                //annotateMat( i, &( conv_mats[i] ), (*frames)[i].getBestBlobs() );
                ////drawConvolutionSpots( i, (*frames)[i].getBestBlobs() );
                //window.render( i, &( conv_mats[i] ) );
            }
            else
            {
                if ( all_binary_mats.size() > 0 && display_type - 3 < (int)all_binary_mats[0].size()  )
                {
                    annotateMat(i, &( all_binary_mats[i][display_type-3] ), (*frames)[i].getBestBlobs() );
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
    
    void Renderer::annotateMat( int index, cv::Mat* mat, std::vector< Blob >& best_blobs )
    {
        for ( unsigned int type = 0; type < best_blobs.size(); type++ )
        {
            if ( best_blobs[type].isInitialized() )
            {
                cv::rectangle( *mat, best_blobs[type].getEncompassingRect(10), Tuner::associated_color[type], 2 );
                if ( best_blobs[type].hasCore() )
                {
                    //cv::rectangle( *mat, best_blobs[type].getCore()->getEncompassingRect(1), {150, 200, 255}, 5 );
                    //cv::rectangle( *mat, best_blobs[type].getEncompassingRect(15), {255, 255, 0}, 5 ); // make the cores a little more visible
                }
                std::ostringstream ss;
                //ss << best_blobs[type].getScore() << ", " << (int)best_blobs[type].getConvolutionAverage() << ", " << best_blobs[type].getArea() << ", " << best_blobs[type].getSize();
                ss << best_blobs[type].getScore() << ", " 
                   << (int)best_blobs[type].getAverageColorScore() << ", " 
                   << (int)best_blobs[type].getAreaScore() << ", " 
                   << (int)best_blobs[type].getSizeScore() << ", "
                   << (int)best_blobs[type].getConvolutionAverageScore();
                cv::putText( *mat, ss.str(), cv::Point(10, 470 - (type * 20)), cv::FONT_HERSHEY_SIMPLEX, 0.5, Tuner::associated_color[type], 2);
            }

            //if ( blob.isInitialized() )
            //{
            //    for ( auto&& blob : blobs[type] )
            //    {
            //        cv::rectangle( *mat, blob.getEncompassingRect(2), Tuner::associated_color[type], 2 );
            //    }
            //}
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

    void Renderer::blobAndCoreToImage( int window_index, std::vector< unsigned char* >& binary_data_2d, Blob& blob )
    {
        if (!blob.isInitialized())
        {
            printf("NO GREEN BLOB!\n");
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

