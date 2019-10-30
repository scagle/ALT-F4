#include "window.hpp"
#include "data_frame.hpp"
#include "input_handler.hpp"
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

    void Window::tempDisplay( std::vector< cv::Mat >* mats )
    {
        for (unsigned int i = 0; i < number_of_images; i++)
        {
            namedWindow( names[i], cv::WINDOW_AUTOSIZE );

            if ( (*mats)[i].empty())
                continue;
            cv::imshow( names[i], (*mats)[i] );
        }

        // Handle key input
        unsigned char key = cv::waitKey(10);
        if ( key != 255 )
        {
            InputHandler::addEvent( key );
        }
    }

    void Window::render( unsigned int index, cv::Mat* mat )
    {
        cv::imshow( names[index], *mat );

        // Handle key input
        unsigned char key = cv::waitKey(10);
        if ( key != 255 )
        {
            InputHandler::addEvent( key );
        }
        printf("Displaying! empty = %s\n", (mat->empty()) ? "true" : "false");
    }
};

