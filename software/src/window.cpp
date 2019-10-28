#include "window.hpp"
#include "data_frame.hpp"
#include <sstream>
#include <opencv2/highgui.hpp>

namespace altf4
{
    // Static Declarations

    // Constructors

    // Methods
    bool Window::initializeWindow()
    {
        for ( int i = 0; i < number_of_images; i++ )
        {
            std::stringstream ss;
            ss << "Camera " << i;
            namedWindow( ss.str(), cv::WINDOW_AUTOSIZE ); // Window to show output images
        }
        return true;
    }

    void Window::display( std::vector< DataFrame >* frames )
    {
        for ( int i = 0; i < number_of_images; i++ )
        {
            std::stringstream ss;
            ss << "Camera " << i;
            cv::imshow( ss.str(), *((*frames)[i].getMat()) );
            
        }
    }
};

