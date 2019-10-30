#include "renderer.hpp"

#include "tuning.hpp"
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
            std::vector< Image >& binary_images = (*frames)[i].getBinaryImages();
            
            if ( image == nullptr || image->isEmpty() )
                continue;

            mats[i] = cv::Mat(image->getRows(), image->getCols(), CV_8UC3, image->getData()->data());
            all_binary_mats[i].clear();
            for ( unsigned int type = 0; type < binary_images.size(); type++ )
            {
                all_binary_mats[i].push_back(cv::Mat(image->getRows(), image->getCols(), CV_8UC1, binary_images[type].getData()->data()));
            }

            if (display_type == 0)
            {
                annotateMat(i, &( (*original_images)[i] ), (*frames)[i].getAllBlobs() );
                window.render( i, &( (*original_images)[i] ) );
            }
            else if (display_type == 1)
            {
                annotateMat(i, &( mats[i] ), (*frames)[i].getAllBlobs() );
                window.render( i, &( mats[i] ) );
            }
            else
            {
                if ( all_binary_mats.size() > 0 && display_type - 2 < (int)all_binary_mats[0].size()  )
                {
                    annotateMat(i, &( all_binary_mats[i][display_type-2] ), (*frames)[i].getAllBlobs() );
                    window.render( i, &( all_binary_mats[i][display_type-2] ) );
                }
                else
                {
                    printf("Invalid display_type '%d'\n", display_type);
                    display_type = 0;
                }
            }
        }
    }
    
    void Renderer::annotateMat( int index, cv::Mat* mat, std::vector< std::vector< Blob > >& blobs )
    {
        for ( unsigned int type = 0; type < blobs.size(); type++ )
        {
            for ( auto&& blob : blobs[type] )
            {
                cv::rectangle( *mat, blob.getEncompassingRect(2), tuning::associated_color[type], 2 );
                printf( "Annotating %d, type = %d\n", index, type );
            }
        }
    }

    bool Renderer::initialize( int number_of_cameras )
    {
        mats.resize( number_of_cameras );
        all_binary_mats.resize( number_of_cameras );
        window.initialize( number_of_cameras );
        return true;
    }
};

