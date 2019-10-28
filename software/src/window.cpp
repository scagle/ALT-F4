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

    void Window::temp( std::vector< Image >* images )
    {
        cv::Mat temp_matrix;
        volatile std::vector< unsigned char >* data = (*images)[0].getData();
        temp_matrix = cv::Mat( (*images)[0].getRows(), (*images)[0].getCols(), CV_8UC3, (*images)[0].getData() );
        if (!temp_matrix.empty())
            cv::imshow( "Camera 1", temp_matrix );
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

