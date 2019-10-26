#include "camera.hpp"
#include "image.hpp"
#include "globals.hpp"
#include <opencv2/videoio.hpp>

namespace altf4
{
    // Static Declarations
    static Image image;

    // Constructors

    // Methods
    Image* Camera::grabImage()
    {
        cap >> matrix_buffer;
        static Image image;
        image.copyMatrix( &matrix_buffer, channels );

        return &image;
    }

    void Camera::initialize( int camera_number )
    {
        cap = cv::VideoCapture(camera_number);    // open the camera located at /dev/videoX
        if (!cap.isOpened())    // check if we succeeded
        {
            printf("*** WARNING: Camera %d can't be opened! (camera.cpp)\n", camera_number);
            printf("             Continuing anyways!\n");
        }
        // Setup VideoCapture settings (if not supported)
        cap.set(cv::CAP_PROP_FPS, FPS)              ; // 30 seems to be maximum. (may hang program depending on cam)
        cap.set(cv::CAP_PROP_FRAME_WIDTH, MAX_COL)  ; // Lowest possible 4:3 aspect ratio
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, MAX_ROW) ; // Lowest possible 4:3 aspect ratio
        cap.set(cv::CAP_PROP_BUFFERSIZE, BUFFERSIZE); // Reduce internal frame buffer size (we have our own)

        cv::Mat temp_matrix;
        cap >> temp_matrix; // get a frame from camera, so we can grab size
        if ( DEBUG == true )
            printStats();

        channels = temp_matrix.channels();
        cols     = temp_matrix.cols;
        width    = temp_matrix.cols;
        rows     = temp_matrix.rows;
        height   = temp_matrix.rows;
        matrix_buffer.resize(width * height * channels);  // initialize 'buffer' to correct size
    }

    void Camera::printStats()
    {
        printf("Camera Settings:\n");
        printf("\tcamera width  / cols: %lf\n", cap.get(cv::CAP_PROP_FRAME_WIDTH) );
        printf("\tcamera height / rows: %lf\n", cap.get(cv::CAP_PROP_FRAME_HEIGHT));
        printf("\tcamera framerate    : %lf\n", cap.get(cv::CAP_PROP_FPS)         );
        printf("\tvideo buffer size   : %lf\n", cap.get(cv::CAP_PROP_BUFFERSIZE)  );
        printf("\tvideo width  / cols : %d\n", width                             );
        printf("\tvideo height / rows : %d\n", height                            );
        printf("\tvideo channels      : %d\n", channels                          );
    }
};

