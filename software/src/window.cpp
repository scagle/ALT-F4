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
        int tune_type;
        int slider;
    };

    void handleTuner( int value, void* dt );
    int displayToTuneType( int display_type );

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
    
    void Window::wait( int wait_time )
    {
        // Handle key input
        unsigned char key = cv::waitKey( wait_time );
        if ( key != 255 )
        {
            InputHandler::addEvent( key );
        }
    }


    void Window::render( unsigned int index, cv::Mat* mat )
    {
        // Show image
        cv::imshow( names[index], *mat );

        // Handle tuning events
        if ( handle_tune )
        {
            handle_tune = false;
            if ( update_tune )
            {
                for ( int i = 0; i < names.size(); i++ )
                {
                    cv::destroyWindow("tune " + names[i]);
                }
            }
            if ( tune || update_tune )
            {
                update_tune = false;
                for ( int i = names.size() - 1; i >= 0; i-- )
                {
                    namedWindow( "tune " + names[i], cv::WINDOW_AUTOSIZE );
                    cv::createTrackbar( "hue_l:", "tune " + names[i], &Tuner::hsv_thresholds[i][tune_type].first.b , 255, nullptr );
                    cv::createTrackbar( "sat_l:", "tune " + names[i], &Tuner::hsv_thresholds[i][tune_type].first.g , 255, nullptr );
                    cv::createTrackbar( "val_l:", "tune " + names[i], &Tuner::hsv_thresholds[i][tune_type].first.r , 255, nullptr );
                    cv::createTrackbar( "hue_h:", "tune " + names[i], &Tuner::hsv_thresholds[i][tune_type].second.b, 255, nullptr );
                    cv::createTrackbar( "sat_h:", "tune " + names[i], &Tuner::hsv_thresholds[i][tune_type].second.g, 255, nullptr );
                    cv::createTrackbar( "val_h:", "tune " + names[i], &Tuner::hsv_thresholds[i][tune_type].second.r, 255, nullptr );
                }
                cv::waitKey(1);
            }
            else
            {
                for ( int i = 0; i < names.size(); i++ )
                {
                    cv::destroyWindow("tune " + names[i]);
                }
            }
        }
        
        // Handle key input
        unsigned char key = cv::waitKey(1);
        if ( key != 255 )
        {
            printf("Input received\n");
            InputHandler::addEvent( key );
        }
    }

    void Window::toggleTune( int display_type )
    {
        this->tune = !tune;
        this->tune_type = displayToTuneType( display_type );
        this->handle_tune = true;
    }

    void Window::updateTune( int display_type )
    {
        if ( this->tune )
        {
            int new_tune = displayToTuneType( display_type );
            if ( this->tune_type != new_tune )
            {
                this->tune_type = new_tune;
                this->update_tune = true;
                this->handle_tune = true;
            }
        }
    }


    int displayToTuneType( int display_type )
    {
        if ( display_type >= 3 && (int)Tuner::hsv_thresholds[0].size() > ( display_type - 3 ) )
        {
            return ( display_type - 3 );
        }
        return 0;
    }

//    void handleTuner( int value, void* dt )
//    {
//        printf( "Handle Event! %d\n", value );
//        int tune_type = *(int*)dt;
//        //std::unique_ptr< TuneArguments >* args = (std::unique_ptr<TuneArguments>*)dt;
//        //int tune_type = (*(*args)).tune_type;
//        //int slider = (*args)->slider;
//        switch ( tune_type )
//        {
//            case 0:
//                Tuner::hsv_thresholds[tune_type].first.b  = (unsigned char)value;
//                break;
//            case 1:
//                Tuner::hsv_thresholds[tune_type].first.g  = (unsigned char)value;
//                break;
//            case 2:
//                Tuner::hsv_thresholds[tune_type].first.r  = (unsigned char)value;
//                break;
//            case 3:
//                Tuner::hsv_thresholds[tune_type].second.b = (unsigned char)value;
//                break;
//            case 4:
//                Tuner::hsv_thresholds[tune_type].second.g = (unsigned char)value;
//                break;
//            case 5:
//                Tuner::hsv_thresholds[tune_type].second.r = (unsigned char)value;
//                break;
//            default:
//                break;
//        }
//        printf("Tune_type: %d\n", tune_type);
//        //printf("Previous hue_l = %d\n", Tuner::hsv_thresholds[tune_type].first.b);
//    }
};

