/*! TODO:
 *  - Figure out how to incorporate exponential moving average to find flashies
 *  - Figure out if we need to cluster groups together (to optimize)
 *  - Figure out how to multithread the frame gathering to seperate it from processing (maybe cores?)
 */
// Webcam Tests
//  Testing how to get input from a webcam using OpenCV

#include <chrono>
#include <deque>
#include <errno.h>
#include <fcntl.h>
#include <fcntl.h> 
#include <sstream>
#include <iomanip>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <termios.h>
#include <thread>
#include <unistd.h>
#include <vector>

#include "serial.hpp"
#include "global.hpp"
#include "frame.hpp"
#include "serial.hpp"
#include "pong/pong.hpp"

#if CV_MAJOR_VERISON <= 2   // Tested using OpenCV 2.4.9 (through apt-get)
    #include <opencv2/video/video.hpp>
    #include <opencv2/imgproc/imgproc.hpp>
    #include <opencv2/highgui/highgui.hpp>
    #include <opencv2/core/core.hpp>
    #include <opencv2/features2d.hpp>
#elif CV_MAJOR_VERISON <= 3   // Tested using OpenCV 3.2.0 (through apt-get)
    #include <opencv2/videoio.hpp>
    #include <opencv2/imgproc.hpp>
    #include <opencv2/highgui.hpp>
    #include <opencv2/core.hpp>
    #include <opencv2/features2d.hpp>
#else                      // For newer versions
//  #include <opencv2/opencv.hpp>  // Includes EVERYTHING
    #include <opencv2/videoio.hpp>
    #include <opencv2/imgproc.hpp>
    #include <opencv2/highgui.hpp>
    #include <opencv2/core.hpp>
    #include <opencv2/features2d.hpp>
#endif

// Defining some Constants
#ifdef HSV
    // values to base scores off of
    const unsigned char blob_bgr[2][3] = { {BLOB_RED_LASER_HSV_HUE   , BLOB_RED_LASER_HSV_SAT   , BLOB_RED_LASER_HSV_VAL  },   // Expected Values
                                           {BLOB_GREEN_LASER_HSV_HUE , BLOB_GREEN_LASER_HSV_SAT , BLOB_GREEN_LASER_HSV_VAL} }; // Expected Values
    const unsigned char edge_bgr[2][3] = { {EDGE_RED_LASER_HSV_HUE   , EDGE_RED_LASER_HSV_SAT   , EDGE_RED_LASER_HSV_VAL  },   // Expected Values
                                           {EDGE_GREEN_LASER_HSV_HUE , EDGE_GREEN_LASER_HSV_SAT , EDGE_GREEN_LASER_HSV_VAL} }; // Expected Values
    const unsigned char core_bgr[2][3] = { {CORE_RED_LASER_HSV_HUE   , CORE_RED_LASER_HSV_SAT   , CORE_RED_LASER_HSV_VAL  },   // Expected Values
                                           {CORE_GREEN_LASER_HSV_HUE , CORE_GREEN_LASER_HSV_SAT , CORE_GREEN_LASER_HSV_VAL} }; // Expected Values
    // filters used to score
    const unsigned int  filters[2]     = { (FILTER_RED_LASER_BLOB_HSV_HUE << 0)   + (FILTER_RED_LASER_BLOB_HSV_SAT << 1)   + (FILTER_RED_LASER_BLOB_HSV_VAL << 2)   + // blob H S V
                                           (FILTER_RED_LASER_EDGE_HSV_HUE << 3)   + (FILTER_RED_LASER_EDGE_HSV_SAT << 4)   + (FILTER_RED_LASER_EDGE_HSV_VAL << 5)   + // edge H S V
                                           (FILTER_RED_LASER_CORE_HSV_HUE << 6)   + (FILTER_RED_LASER_CORE_HSV_SAT << 7)   + (FILTER_RED_LASER_CORE_HSV_VAL << 8)   + // core H S V
                                           (FILTER_RED_LASER_HSV_SIZE << 9),                                                                                          // size
                                           (FILTER_GREEN_LASER_BLOB_HSV_HUE << 0) + (FILTER_GREEN_LASER_BLOB_HSV_SAT << 1) + (FILTER_GREEN_LASER_BLOB_HSV_VAL << 2) + // blob H S V
                                           (FILTER_GREEN_LASER_EDGE_HSV_HUE << 3) + (FILTER_GREEN_LASER_EDGE_HSV_SAT << 4) + (FILTER_GREEN_LASER_EDGE_HSV_VAL << 5) + // edge H S V
                                           (FILTER_GREEN_LASER_CORE_HSV_HUE << 6) + (FILTER_GREEN_LASER_CORE_HSV_SAT << 7) + (FILTER_GREEN_LASER_CORE_HSV_VAL << 8) + // core H S V
                                           (FILTER_GREEN_LASER_HSV_SIZE << 9)                                                                                         // size
                                         };
    // threshold used to get binary matrices
    int threshold_values[2][2][3]      = { { { RED_LASER_HSV_HUE_LOW,    RED_LASER_HSV_SAT_LOW ,   RED_LASER_HSV_VAL_LOW    }, 
                                             { RED_LASER_HSV_HUE_HIGH,   RED_LASER_HSV_SAT_HIGH,   RED_LASER_HSV_VAL_HIGH   } }, 
                                           { { GREEN_LASER_HSV_HUE_LOW,  GREEN_LASER_HSV_SAT_LOW , GREEN_LASER_HSV_VAL_LOW  }, 
                                             { GREEN_LASER_HSV_HUE_HIGH, GREEN_LASER_HSV_SAT_HIGH, GREEN_LASER_HSV_VAL_HIGH } }  
                                         };

#else
    // values to base scores off of
    const unsigned char blob_bgr[2][3] = { {BLOB_RED_LASER_BGR_BLUE   , BLOB_RED_LASER_BGR_GREEN   , BLOB_RED_LASER_BGR_RED  },   // Expected Values
                                           {BLOB_GREEN_LASER_BGR_BLUE , BLOB_GREEN_LASER_BGR_GREEN , BLOB_GREEN_LASER_BGR_RED} }; // Expected Values
    const unsigned char edge_bgr[2][3] = { {EDGE_RED_LASER_BGR_BLUE   , EDGE_RED_LASER_BGR_GREEN   , EDGE_RED_LASER_BGR_RED  },   // Expected Values
                                           {EDGE_GREEN_LASER_BGR_BLUE , EDGE_GREEN_LASER_BGR_GREEN , EDGE_GREEN_LASER_BGR_RED} }; // Expected Values
    const unsigned char core_bgr[2][3] = { {CORE_RED_LASER_BGR_BLUE   , CORE_RED_LASER_BGR_GREEN   , CORE_RED_LASER_BGR_RED  },   // Expected Values
                                           {CORE_GREEN_LASER_BGR_BLUE , CORE_GREEN_LASER_BGR_GREEN , CORE_GREEN_LASER_BGR_RED} }; // Expected Values
    // filters used to score
    const unsigned int  filters[2]     = { (FILTER_RED_LASER_BLOB_BGR_BLUE << 0)   + (FILTER_RED_LASER_BLOB_BGR_GREEN << 1)   + (FILTER_RED_LASER_BLOB_BGR_RED << 2)   + // blob B G R
                                           (FILTER_RED_LASER_EDGE_BGR_BLUE << 3)   + (FILTER_RED_LASER_EDGE_BGR_GREEN << 4)   + (FILTER_RED_LASER_EDGE_BGR_RED << 5)   + // edge B G R
                                           (FILTER_RED_LASER_CORE_BGR_BLUE << 6)   + (FILTER_RED_LASER_CORE_BGR_GREEN << 7)   + (FILTER_RED_LASER_CORE_BGR_RED << 8)   + // core B G R
                                           (FILTER_RED_LASER_BGR_SIZE << 9),                                                                                             // size
                                           (FILTER_GREEN_LASER_BLOB_BGR_BLUE << 0) + (FILTER_GREEN_LASER_BLOB_BGR_GREEN << 1) + (FILTER_GREEN_LASER_BLOB_BGR_RED << 2) + // blob B G R
                                           (FILTER_GREEN_LASER_EDGE_BGR_BLUE << 3) + (FILTER_GREEN_LASER_EDGE_BGR_GREEN << 4) + (FILTER_GREEN_LASER_EDGE_BGR_RED << 5) + // edge B G R
                                           (FILTER_GREEN_LASER_CORE_BGR_BLUE << 6) + (FILTER_GREEN_LASER_CORE_BGR_GREEN << 7) + (FILTER_GREEN_LASER_CORE_BGR_RED << 8) + // core B G R
                                           (FILTER_GREEN_LASER_BGR_SIZE << 9)                                                                                            // size
                                         };
    // threshold used to get binary matrices
    int threshold_values[2][2][3]      = { { { RED_LASER_BGR_BLUE_LOW,    RED_LASER_BGR_GREEN_LOW ,   RED_LASER_BGR_RED_LOW    }, 
                                             { RED_LASER_BGR_BLUE_HIGH,   RED_LASER_BGR_GREEN_HIGH,   RED_LASER_BGR_RED_HIGH   } }, 
                                           { { GREEN_LASER_BGR_BLUE_LOW,  GREEN_LASER_BGR_GREEN_LOW , GREEN_LASER_BGR_RED_LOW  }, 
                                             { GREEN_LASER_BGR_BLUE_HIGH, GREEN_LASER_BGR_GREEN_HIGH, GREEN_LASER_BGR_RED_HIGH } }  
                                         };

#endif

using namespace cv;
using namespace std;

VideoCapture initializeVideo();
void printStats(VideoCapture, Mat);
int processInputs(Mat);
void on_adjust(int, void*);
void serialWriteCoordinates(int green_x, int green_y, int red_x, int red_y);
std::string intToString ( int number );
void serial_thread();

const string capture_name = "capture.png"; // 'yanked' photo's name in filesystem
deque<Frame>            frames(FRAME_FIFO_SIZE); // FIFO queue to store frames in

int green_x = 0;
int green_y = 0;
int red_x = 0;
int red_y = 0;
std::mutex mtx;           // mutex for critical section
int cont = 1; // continue variable 

// Pong initializations
Pong pong;
SerialHandler sh;
Frame new_frame;

int main(int, char**)
{
    VideoCapture cap = initializeVideo();
    // Variable Declaration
    Mat orig_mat, edited_mat, output_mat;   // Original Image, Modified Image, Output Image
    namedWindow("output", WINDOW_AUTOSIZE); // Window to show output images
    namedWindow("tuning", WINDOW_AUTOSIZE); // Window to show tuning images

    std::string path = "/dev/ttyACM0";
    if ( sh.openUART( path ) == false )
    {
        printf( "Error opening '%s'\n", path.c_str() );
    }
    else
    {
        printf("UART Initialization Success!\nEnabling UART!\n");
        sh.set_interface_attribs( B115200, 0 );   // set speed to 115,200 bps, 8n1 (no parity)
        sh.set_blocking( 0 );                     // set no blocking
    }
    std::thread st( serial_thread );

    try { 
        // Frame-Size Dependant Variables
        cap >> orig_mat; // get a frame from camera, so we can grab size
        printStats(cap, orig_mat);

        int channels = orig_mat.channels();
        int cols     = orig_mat.cols;
        int width    = orig_mat.cols;
        int rows     = orig_mat.rows;
        int height   = orig_mat.rows;
        vector< unsigned char > frame_data(width * height * channels);  // initialize 'frame_data' to correct size

        createTrackbar( "b_l:", "tuning", &threshold_values[1][0][0], 255, on_adjust );
        createTrackbar( "g_l:", "tuning", &threshold_values[1][0][1], 255, on_adjust );
        createTrackbar( "r_l:", "tuning", &threshold_values[1][0][2], 255, on_adjust );
        createTrackbar( "b_h:", "tuning", &threshold_values[1][1][0], 255, on_adjust );
        createTrackbar( "g_h:", "tuning", &threshold_values[1][1][1], 255, on_adjust );
        createTrackbar( "r_h:", "tuning", &threshold_values[1][1][2], 255, on_adjust );

        while ( cont == 1 )
        {
            //// Capture Video
            cap >> orig_mat; // get a frame from camera
            edited_mat = orig_mat.clone(); // preserve original
            output_mat = orig_mat.clone(); // preserve original
            if (HSV) // If using HSV instead of BGR
                cvtColor(edited_mat, edited_mat, COLOR_BGR2HSV); // convert to hsv

            // Convert OpenCV "Mat" to a 1D vector
            frame_data.assign(edited_mat.data, edited_mat.data + edited_mat.total() * channels); 

            //// Manipulate FIFO Queue
            frames.pop_back();                                            // Remove last Frame from queue

            mtx.lock(); // Lock mutex, because uart thread wants to use these values

            new_frame = Frame(frame_data, width, height, channels); // Create a new Frame based on frame_data

            frames.push_front(new_frame);                                 // Add the new Frame to the front of queue

            //// Blob Detection
            // Frame looks at pixels, applyings min/max threshold, and creates binary matrix
            new_frame.inRange(threshold_values[0][0][0], threshold_values[0][1][0], threshold_values[0][0][1], threshold_values[0][1][1], threshold_values[0][0][2], threshold_values[0][1][2], 0); 
            new_frame.inRange(threshold_values[1][0][0], threshold_values[1][1][0], threshold_values[1][0][1], threshold_values[1][1][1], threshold_values[1][0][2], threshold_values[1][1][2], 1); 
            new_frame.findBlobs(0);  // Find Blobs based on red binary matrix results
            new_frame.findBlobs(1);  // Find Blobs based on green binary matrix results

            mtx.unlock(); // Lock mutex, because uart thread wants to use these values

            if ( new_frame.hasBlobs(1) == false )
            {
                printf("MAIN_THREAD has no blobs\n");
            }
            Scalar colors[2] = {Scalar(0, 0, 255), Scalar(0, 255, 0)};
            for (int laser_color = 0; laser_color < 2; laser_color++)
            {
                if (new_frame.hasBlobs(laser_color))
                {
                    // Identify the blob with the highest score
                    Blob best_blob = new_frame.bestBlob(filters[laser_color], blob_bgr[laser_color], edge_bgr[laser_color], core_bgr[laser_color], laser_color);
                    if (best_blob.isInitialized())
                    {
                        // Draw Best Blob's Boundary
                        Rect brect = best_blob.getRect();

                        mtx.lock(); // Lock mutex, because uart thread wants to use these values
                        if ( laser_color == 0 )
                        {
                            red_x = brect.x + brect.width / 2;   // Get Center Position
                            red_y = brect.y + brect.height / 2;  // Get Center Position
                        }
                        else
                        {
                            green_x = brect.x + brect.width / 2;  // Get Center Position
                            green_y = brect.y + brect.height / 2; // Get Center Position
                        }
                        mtx.unlock(); // Unlock mutex, so that uart thread can start using these values

                        rectangle(output_mat, Rect(brect.x - 10, brect.y - 10, brect.width + 20, brect.height + 20), colors[laser_color], 2);
                        putText(output_mat, 
                                "#: " + to_string(best_blob.getBlobPixels().size()) + " S:" + to_string(best_blob.getScore()), 
                                Point(brect.x, brect.y - 20), FONT_HERSHEY_SIMPLEX, 0.5, colors[laser_color], 2);

                        // Display info at bottom of screen
                        Scalar average_bgr = best_blob.getAverageBGR(0);
                        Scalar edge_bgr = best_blob.getAverageBGR(1);
                        Scalar core_bgr = best_blob.getAverageBGR(2);
                        printf("BLOB: (%d, %d, %d)\nEDGE: (%d, %d, %d)\nCORE: (%d, %d, %d)\n", (int)average_bgr[0], (int)average_bgr[1], (int)average_bgr[2],
                                                                                               (int)edge_bgr[0], (int)edge_bgr[1], (int)edge_bgr[2],
                                                                                               (int)core_bgr[0], (int)core_bgr[1], (int)core_bgr[2]  );

                        string average_bgr_str = "#Blobs: " + to_string(new_frame.getBlobs(laser_color).size()) + 
                                                 " BGR: ("  + to_string((int)average_bgr[0])         + 
                                                       ", " + to_string((int)average_bgr[1])         + 
                                                       ", " + to_string((int)average_bgr[2])         + ")";
                        putText(output_mat, average_bgr_str, Point(10, MAX_ROW - 10 - (20 * laser_color)), FONT_HERSHEY_SIMPLEX, 0.5, colors[laser_color], 2);

                        // Update Pong Paddle
                        if (laser_color == 0)
                            pong.updatePaddle(brect.y, 0);
                        if (laser_color == 1)
                            pong.updatePaddle(brect.y, 1);
                    }
                }
            }

            // Display Pong
            if (pong.isRunning())
            {
                pong.update();
                rectangle(output_mat, pong.getBall(),    Scalar(BALL_BLUE, BALL_GREEN, BALL_RED), 2);
                rectangle(output_mat, pong.getPaddle(0), Scalar(PADDLE_LEFT_BLUE, PADDLE_LEFT_GREEN, PADDLE_LEFT_RED), 2);
                rectangle(output_mat, pong.getPaddle(1), Scalar(PADDLE_RIGHT_BLUE, PADDLE_RIGHT_GREEN, PADDLE_RIGHT_RED), 2);
                if (pong.isLost())
                {
                    Scalar text_color = (pong.getLostTeam() == 0) ? Scalar(50, 50, 255) : Scalar(50, 255, 50);
                    putText(output_mat, "Lost!", Point(MAX_COL / 2 - 50, MAX_ROW / 2 - 10), FONT_HERSHEY_SIMPLEX, 1, text_color, 2);
                }
            }

            Mat frame_mat_red = new_frame.getMat(1, 1);        // Get OpenCV binary image 
            Mat frame_mat_green = new_frame.getMat(1, 2);      // Get OpenCV binary image 
            Mat resized_orig;                                  // Image to write resized output_image
            resize(output_mat, resized_orig, Size(1040, 780)); // Actually resize output_image

            imshow("tuning", frame_mat_red);
            imshow("tuning", frame_mat_green);
            imshow("output", resized_orig);

            cont = processInputs(output_mat);
        }
    }
    catch (int e){
        cout << "An Exception occurred. Exception #" << e << "\n";
    }
    st.join();
    cout << "Goodbye!\n";

    // Camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

void serial_thread() 
{
    while ( cont == 1 )
    {
        auto begin = std::chrono::steady_clock::now();
        mtx.lock();
        int gx = green_x;
        int gy = green_y;
        int rx = red_x;
        int ry = red_y;
        bool green_laser_detected = new_frame.hasBlobs(1);
        mtx.unlock();
        printf("Writing to UART...\n");
        if ( green_laser_detected )
        {
            sh.writeString( "strt\r" );              
            std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );
            sh.writeNumber( green_x );                   
            std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );
            sh.writeNumber( green_y );                   
            std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );
            sh.writeNumber( red_x );                  
            std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );
            sh.writeNumber( red_y );                  
            std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );
            sh.writeString( "stop\r" );              
            std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );
        }
        else
        {
            printf("\nNO BLOBS!!!\n");
            sh.writeString( "strt\r" );              
            std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );
            sh.writeString( "none\r" );              
            std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );
            sh.writeString( "stop\r" );              
            std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );
        }
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin );
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) - duration );
    }
}

VideoCapture initializeVideo()
{
    VideoCapture cap(CAMERA_NUMBER);    // open the camera located at /dev/videoX
    if (!cap.isOpened())    // check if we succeeded
    {
        cout << "Camera can't be opened, exiting!\n";
        return -1;
    }
    // Setup VideoCapture settings (if not supported)
    cout << "Setting up Camera settings..." << "\r";
    cout.flush(); // actually print out (output is normally buffered)
    cap.set(CAP_PROP_FPS, FPS)              ; // 30 seems to be maximum. (may hang program depending on cam)
    cap.set(CAP_PROP_FRAME_WIDTH, MAX_COL)  ; // Lowest possible 4:3 aspect ratio
    cap.set(CAP_PROP_FRAME_HEIGHT, MAX_ROW) ; // Lowest possible 4:3 aspect ratio
    cap.set(CAP_PROP_BUFFERSIZE, BUFFERSIZE); // Reduce internal frame buffer size (we have our own)
    cout << "Camera Settings Successfully Initialized!" << "\n";
    return cap;
}

void printStats(VideoCapture cap, Mat img)
{
    if (DEBUG)
    {
        unsigned int width = img.cols;
        unsigned int height = img.rows;  
        unsigned int step = img.step;           // Full row width in bytes (so width * 8?)
        unsigned int channels = img.channels(); // How many channels does the images have (rgb = 3)
        cout << "Camera Settings:" << "\n";
        cout << "\tcamera width  / cols: " << cap.get(CAP_PROP_FRAME_WIDTH)  << "\n";
        cout << "\tcamera height / rows: " << cap.get(CAP_PROP_FRAME_HEIGHT) << "\n";
        cout << "\tcamera framerate    : " << cap.get(CAP_PROP_FPS)          << "\n";
        cout << "\tvideo buffer size   : " << cap.get(CAP_PROP_BUFFERSIZE)   << "\n";
        cout << "\tvideo width  / cols : " << width                          << "\n";
        cout << "\tvideo height / rows : " << height                         << "\n";
        cout << "\tvideo channels      : " << channels                       << "\n";
        cout << "\tFIFO buffer size    : " << FRAME_FIFO_SIZE                << "\n";
    }
}

int processInputs(Mat yank_mat)
{
    // Delay, and Handle character inputs
    unsigned char key = waitKey(1); 
    switch (key)
    {
        case 'y':  // (Yank) Capture a still of original frame, save it to file, and display it on another window
            {
                Mat captured_image; // Original, monochrome (grayscale), and hue/sat/val matrixes
                imwrite(capture_name, yank_mat);
                captured_image = imread(capture_name, 1);
                imshow("yanked", yank_mat);
                break;
            }
        case 's':  // Capture a screenshot image, and save it to screenshots/ directory   
            {
                int error = system("scrot -e 'mv $f ../screenshots/'"); // Capture screenshot
                if (error)
                    cout << "ERROR: Screen Shot failed! (do you have the 'scrot' package installed?)" << "\n";
                else
                    cout << "Screen Shot saved!" << "\n";
                break;
            }
        case 'p':  // Play Pong!
            {
                cout << "Lets goooo!\n";
                pong.restart();
                if (pong.isRunning())
                    pong.stop();
                else
                    pong.start();
                break;
            }
        case 'r':  // flash leds
            {
                sh.writeString( "r\r" );
                break;
            }
        case 'g':  // flash leds
            {
                sh.writeString( "g\r" );
                break;
            }
        case 'b':  // flash leds
            {
                sh.writeString( "b\r" );
                break;
            }
    }
    if (key == 'q')
    {
        return -1;
    }
    return 1;
}

void on_adjust(int, void* ) {}
