#include "post_process.hpp"

namespace altf4
{
    // Static Declarations

    // Constructors

    // Methods
    void PostProcess::renderMats( std::vector< DataFrame >* frames )
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
        }
    }
    bool PostProcess::initialize( int number_of_cameras )
    {
        mats.resize( number_of_cameras );
        all_binary_mats.resize( number_of_cameras );
        return true;
    }
};

