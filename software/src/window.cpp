#include "window.hpp"
#include "data_frame.hpp"
#include <sstream>
#include <opencv2/highgui.hpp>

namespace altf4
{
    // Static Declarations

    // Constructors

    // Methods
    bool Window::initialize( unsigned int number_of_images )
    {
        this->number_of_images = number_of_images;
        names.resize(number_of_images);
        matrices.resize(number_of_images);
        for (unsigned int i = 0; i < number_of_images; i++)
        {
            std::stringstream ss;
            ss << "Camera " << i;
            names[i] = ss.str();
        }
        return true;
    }

    void Window::tempDisplay( std::vector< DataFrame >& frames )
    {
        for (unsigned int i = 0; i < number_of_images; i++)
        {
            namedWindow( names[i], cv::WINDOW_AUTOSIZE );
            Image* image = frames[i].getImage();
            std::vector< Image > binary_images = frames[i].getBinaryImages();
            if ( image == nullptr || image->isEmpty() )
                continue;

            delete matrices[i];
            //matrices[i] = new cv::Mat(480, 640, CV_8UC3, image->getData()->data());
            matrices[i] = new cv::Mat(480, 640, CV_8UC1, binary_images[0].getData()->data());
            

            //matrices[i] = (*images)[i].getMatrix();
            //if (matrices[i]->empty())
            //    continue;
            cv::imshow( names[i], *(matrices[i]) );
        }
        cv::waitKey(50);
    }

    void Window::display( std::vector< DataFrame >& frames )
    {
        for ( unsigned int i = 0; i < number_of_images; i++ )
        {
            //cv::imshow( names[i], *(frames[i].getMat()) );
            
        }
    }
};

