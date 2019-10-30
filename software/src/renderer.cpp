#include "renderer.hpp"
#include <opencv2/videoio.hpp>

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
            for ( unsigned int t = 0; t < binary_images.size(); t++ )
            {
                all_binary_mats[i].push_back(cv::Mat(image->getRows(), image->getCols(), CV_8UC1, binary_images[t].getData()->data()));
            }

            if (type == 0)
            {
                annotateMat(i, &( (*original_images)[i] ) );
                window.render( i, &( (*original_images)[i] ) );
            }
            else if (type == 1)
            {
                annotateMat(i, &( mats[i] ) );
                window.render( i, &( mats[i] ) );
            }
            else
            {
                if ( all_binary_mats.size() > 0 && type - 2 < all_binary_mats[0].size()  )
                {
                    printf("Type '%d'\n", type);
                    annotateMat(i, &( all_binary_mats[i][type-2] ) );
                    window.render( i, &( all_binary_mats[i][type-2] ) );
                }
                else
                {
                    printf("Invalid type '%d'\n", type);
                    type = 0;
                }
            }
        }
    }
    
    void Renderer::annotateMat( int index, cv::Mat* mat )
    {
        printf("Annotating %d, type = %d\n", index, type);
    }

    bool Renderer::initialize( int number_of_cameras )
    {
        mats.resize( number_of_cameras );
        all_binary_mats.resize( number_of_cameras );
        window.initialize( number_of_cameras );
        return true;
    }
};

