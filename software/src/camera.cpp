#include "camera.hpp"
#include "image.hpp"
#include "globals.hpp"
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <chrono>
#include <iostream>
#include <mutex>
#include <fstream>

namespace altf4
{
    // Static Declarations
    std::mutex capture_mutex;

    // Constructors

    // Methods
    std::pair< cv::Mat3b, Image >* Camera::grabImage()
    {
        cap >> matrix_buffer;
        image_pair.first = matrix_buffer.clone();    // Keep original
        cv::cvtColor(matrix_buffer, matrix_buffer, cv::COLOR_BGR2HSV); // convert to hsv
        image_pair.second = Image(&matrix_buffer, rows, cols, channels);

        return &image_pair;
    }

    bool Camera::initialize( int camera_number )
    {
        try
        {
            this->camera_number = camera_number;
            std::unique_lock< std::mutex> ul( capture_mutex );
            readOS();
            if ( os == "Ubuntu" )
                cap = cv::VideoCapture(camera_number);    // open the camera located at /dev/videoX
            else if ( os == "Arch Linux" )
                cap = cv::VideoCapture(camera_number * 2);    // open the camera located at /dev/videoX

            if (!cap.isOpened())    // check if we succeeded
            {
                printf("*** WARNING: Camera %d can't be opened! (camera.cpp)\n", camera_number);
                return false;
            }
            ul.unlock();
            // Setup VideoCapture settings (if not supported)
            cap.set(cv::CAP_PROP_FPS, FPS)              ; // 30 seems to be maximum. (may hang program depending on cam)
            cap.set(cv::CAP_PROP_FRAME_WIDTH, MAX_COL)  ; // Lowest possible 4:3 aspect ratio
            cap.set(cv::CAP_PROP_FRAME_HEIGHT, MAX_ROW) ; // Lowest possible 4:3 aspect ratio
            //cap.set(cv::CAP_PROP_AUTO_WB, 1)            ; // Enable auto white balance
            //cap.set(cv::CAP_PROP_BUFFERSIZE, BUFFERSIZE); // Reduce internal frame buffer size (we have our own)

            cv::Mat temp_matrix;
            cap >> temp_matrix; // get a frame from camera, so we can grab size
            channels = temp_matrix.channels();
            cols     = temp_matrix.cols;
            rows     = temp_matrix.rows;
            if ( DEBUG == true )
                printStats();
        }
        catch (const std::exception &e)
        {
            printf("*** WARNING: Error encountered when trying to open Camera %d (camera.cpp)\n\t Error = %s\n", camera_number, e.what());

        }

        matrix_buffer.resize(rows * cols * channels);  // initialize 'buffer' to correct size
        return true;
    }

    void Camera::printStats()
    {
        printf("Camera Settings:\n");
        printf("\tcamera width  / cols: %lf\n", cap.get(cv::CAP_PROP_FRAME_WIDTH) );
        printf("\tcamera height / rows: %lf\n", cap.get(cv::CAP_PROP_FRAME_HEIGHT));
        printf("\tcamera framerate    : %lf\n", cap.get(cv::CAP_PROP_FPS)         );
        //printf("\tcamera auto-whitebal: %lf\n", cap.get(cv::CAP_PROP_AUTO_WB)     );
        //printf("\tvideo buffer size   : %lf\n", cap.get(cv::CAP_PROP_BUFFERSIZE)  );
        printf("\tvideo width  / cols : %d\n", cols                             );
        printf("\tvideo height / rows : %d\n", rows                            );
        printf("\tvideo channels      : %d\n", channels                          );
    }
    
    void Camera::readOS()
    {
        std::ifstream release("/etc/os-release");
        std::string line;
        if ( release.is_open() )
        {
            while ( getline( release, line ) )
            {
                if ( line.find( "NAME" ) != -1 )
                {
                    int start = line.find("\"") + 1;
                    int end = line.find("\"", start);
                    this->os = line.substr(start, end-start);
                    printf("line: '%d', '%d'\n", start, end);
                    printf("Detected Operating System: %s\n", this->os.c_str());
                    break;
                }
            }
            release.close();
        }
    }
};

