#include "window.hpp"
#include "data_frame.hpp"
#include "input_handler.hpp"
#include "tuner.hpp"
#include <sstream>
#include <memory>
#include <opencv2/highgui.hpp>

namespace altf4
{
    // Static Declarations
    struct TuneArguments 
    {
        int display_type;
        int slider;
    };

    void handleTuner( int value, void* dt );

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
    }

    void Window::toggleTune( int display_type )
    {
        this->tune = !tune;
        if ( tune )
        {
            namedWindow( "tuning", cv::WINDOW_AUTOSIZE );
            cv::waitKey(1);
            printf("Here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

            std::unique_ptr< TuneArguments > args0( new TuneArguments{ display_type, 0 } );
            std::unique_ptr< TuneArguments > args1( new TuneArguments{ display_type, 1 } );
            std::unique_ptr< TuneArguments > args2( new TuneArguments{ display_type, 2 } );
            std::unique_ptr< TuneArguments > args3( new TuneArguments{ display_type, 3 } );
            std::unique_ptr< TuneArguments > args4( new TuneArguments{ display_type, 4 } );
            std::unique_ptr< TuneArguments > args5( new TuneArguments{ display_type, 5 } );
            cv::createTrackbar( "hue_l:", "tuning", nullptr, 255, handleTuner, &args0 );
            cv::createTrackbar( "sat_l:", "tuning", nullptr, 255, handleTuner, &args1 );
            cv::createTrackbar( "val_l:", "tuning", nullptr, 255, handleTuner, &args2 );
            cv::createTrackbar( "hue_h:", "tuning", nullptr, 255, handleTuner, &args3 );
            cv::createTrackbar( "sat_h:", "tuning", nullptr, 255, handleTuner, &args4 );
            cv::createTrackbar( "val_h:", "tuning", nullptr, 255, handleTuner, &args5 );
        }
        else
        {
            cv::destroyWindow( "tuning" );
        }
    }

    void handleTuner( int value, void* dt )
    {
        std::unique_ptr< TuneArguments >* args = (std::unique_ptr<TuneArguments>*)dt;
        int display_type = (*args)->display_type;
        int slider = (*args)->slider;
        switch ( (*args)->display_type )
        {
            case 0:
                Tuner::hsv_thresholds[display_type].first.b  = (unsigned char)value;
                break;
            case 1:
                Tuner::hsv_thresholds[display_type].first.g  = (unsigned char)value;
                break;
            case 2:
                Tuner::hsv_thresholds[display_type].first.r  = (unsigned char)value;
                break;
            case 3:
                Tuner::hsv_thresholds[display_type].second.b = (unsigned char)value;
                break;
            case 4:
                Tuner::hsv_thresholds[display_type].second.g = (unsigned char)value;
                break;
            case 5:
                Tuner::hsv_thresholds[display_type].second.r = (unsigned char)value;
                break;
            default:
                break;
        }
    }
};

