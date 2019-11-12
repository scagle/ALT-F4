#include "renderer.hpp"

#include "tuner.hpp"
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

namespace altf4
{
    // Static Declarations

    // Constructors

    // Methods
    void Renderer::renderMats( std::vector< DataFrame >* frames, std::vector< cv::Mat3b >* original_images )
    {
        for ( unsigned int i = 0; i < frames->size(); i++ )
        {
            Image* image = (*frames)[i].getImage();
            std::vector< unsigned char >& conv_data_1d = (*frames)[i].getConvData1D();
            std::vector< Image >& binary_images = (*frames)[i].getBinaryImages();
            
            if ( image == nullptr || image->isEmpty() )
                continue;

            int rows = image->getRows();
            int cols = image->getCols();

            std::vector< unsigned char > test(rows * cols, 0);
            mats.push_back(cv::Mat(rows, cols, CV_8UC3, image->getData()->data()));
            conv_mats.push_back(cv::Mat(rows, cols, CV_8UC1, conv_data_1d.data()));
            all_binary_mats[i].clear();
            for ( unsigned int type = 0; type < binary_images.size(); type++ )
            {
                all_binary_mats[i].push_back(cv::Mat(rows, cols, CV_8UC1, binary_images[type].getData()->data()));
            }

            if (display_type == 0)
            {
                annotateMat(i, &( (*original_images)[i] ), (*frames)[i].getBestBlobs() );
                window.render( i, &( (*original_images)[i] ) );
            }
            else if (display_type == 1)
            {
                annotateMat(i, &( mats[i] ), (*frames)[i].getBestBlobs() );
                window.render( i, &( mats[i] ) );
            }
            else if (display_type == 2)
            {
                annotateMat(i, &( mats[i] ), (*frames)[i].getBestBlobs() );
                window.render( i, &( conv_mats[i] ) );
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
                    cv::rectangle( *mat, best_blobs[type].getCore()->getEncompassingRect(1), {150, 200, 255}, 5 );
                    cv::rectangle( *mat, best_blobs[type].getEncompassingRect(15), {255, 255, 0}, 5 ); // make the cores a little more visible
                }
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
};

