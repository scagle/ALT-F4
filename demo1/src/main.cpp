/*! TODO:
 *  - Figure out how to incorporate exponential moving average to find flashies
 *  - Figure out if we need to cluster groups together (to optimize)
 *  - Figure out how to multithread the frame gathering to seperate it from processing (maybe cores?)
 */
// Webcam Tests
//  Testing how to get input from a webcam using OpenCV

#include <iostream>
#include <deque>
#include <vector>
#include <string>

#include "global.h"
#include "frame.h"
#include "pong/pong.h"

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

// Constants
#ifdef HSV
    const unsigned char blob_bgr[] = {BLOB_HSV_HUE, BLOB_HSV_SAT, BLOB_HSV_VAL}; // 
    const unsigned char edge_bgr[] = {EDGE_HSV_HUE, EDGE_HSV_SAT, EDGE_HSV_VAL}; // 
    const unsigned char core_bgr[] = {CORE_HSV_HUE, CORE_HSV_SAT, CORE_HSV_VAL}; // 
    const unsigned int  filters    = (FILTER_BLOB_HSV_HUE << 0) + (FILTER_BLOB_HSV_SAT << 1) + (FILTER_BLOB_HSV_VAL << 2) +    // blob H S V
                                     (FILTER_EDGE_HSV_HUE << 3) + (FILTER_EDGE_HSV_SAT << 4) + (FILTER_EDGE_HSV_VAL << 5) +    // edge H S V
                                     (FILTER_CORE_HSV_HUE << 6) + (FILTER_CORE_HSV_SAT << 7) + (FILTER_CORE_HSV_VAL << 8) +    // core H S V
                                     (FILTER_SIZE << 9);                                                                       // size
    int threshold_values[2][3]     = { {HSV_HUE_LOW, HSV_SAT_LOW , HSV_VAL_LOW}, {HSV_HUE_HIGH, HSV_SAT_HIGH, HSV_VAL_HIGH} }; // low/high threshold 

#else
    const unsigned char blob_bgr[] = {BLOB_BGR_BLUE, BLOB_BGR_GREEN, BLOB_BGR_RED}; // red-white
    const unsigned char edge_bgr[] = {EDGE_BGR_BLUE, EDGE_BGR_GREEN, EDGE_BGR_RED}; // red
    const unsigned char core_bgr[] = {CORE_BGR_BLUE, CORE_BGR_GREEN, CORE_BGR_RED}; // white
    const unsigned int filters     = (FILTER_BLOB_BGR_BLUE << 0) + (FILTER_BLOB_BGR_GREEN << 1) + (FILTER_BLOB_BGR_RED << 2) +       // blob B G R
                                     (FILTER_EDGE_BGR_BLUE << 3) + (FILTER_EDGE_BGR_GREEN << 4) + (FILTER_EDGE_BGR_RED << 5) +       // edge B G R
                                     (FILTER_CORE_BGR_BLUE << 6) + (FILTER_CORE_BGR_GREEN << 7) + (FILTER_CORE_BGR_RED << 8) +       // core B G R
                                     (FILTER_SIZE << 9);                                                                             // size
    int threshold_values[2][3]     = { {BGR_BLUE_LOW, BGR_GREEN_LOW , BGR_RED_LOW}, {BGR_BLUE_HIGH, BGR_GREEN_HIGH, BGR_RED_HIGH} }; // low/high threshold 
#endif

using namespace cv;
using namespace std;

VideoCapture initializeVideo();
void printStats(VideoCapture, Mat);
int processInputs(Mat);
void on_adjust(int, void*);

const string capture_name = "capture.png"; // 'yanked' photo's name in filesystem
deque<Frame>            frames(FRAME_FIFO_SIZE); // FIFO queue to store frames in

// Pong initializations
Pong pong;

int main(int, char**)
{
    VideoCapture cap = initializeVideo();
    // Variable Declaration
    Mat orig_mat, edited_mat, output_mat; // Original Image, Modified Image, Output Image
    namedWindow("output", WINDOW_AUTOSIZE); // Window to show output images
    namedWindow("tuning", WINDOW_AUTOSIZE); // Window to show tuning images

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

        createTrackbar( "b_l:", "tuning", &threshold_values[0][0], 255, on_adjust );
        createTrackbar( "g_l:", "tuning", &threshold_values[0][1], 255, on_adjust );
        createTrackbar( "r_l:", "tuning", &threshold_values[0][2], 255, on_adjust );
        createTrackbar( "b_h:", "tuning", &threshold_values[1][0], 255, on_adjust );
        createTrackbar( "g_h:", "tuning", &threshold_values[1][1], 255, on_adjust );
        createTrackbar( "r_h:", "tuning", &threshold_values[1][2], 255, on_adjust );

        int cont = 1; // continue variable 
        while (cont == 1)
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
            Frame new_frame = Frame(frame_data, width, height, channels); // Create a new Frame based on frame_data
            frames.push_front(new_frame);                                 // Add the new Frame to the front of queue

            //// Blob Detection
            // Frame looks at pixels, applyings min/max threshold, and creates binary matrix
            new_frame.inRange(threshold_values[0][0],  // threshold B/H low
                              threshold_values[1][0],  // threshold B/H high
                              threshold_values[0][1],  // threshold G/S low
                              threshold_values[1][1],  // threshold G/S high
                              threshold_values[0][2],  // threshold R/V low
                              threshold_values[1][2]); // threshold R/V high
            new_frame.findBlobs();  // Find Blobs based on binary matrix results

            //// Blob Selection
            if (new_frame.hasBlobs())
            {
                // Identify the blob with the highest score
                Blob best_blob = new_frame.bestBlob(filters, blob_bgr, edge_bgr, core_bgr);
                if (best_blob.isInitialized())
                {
                    // Draw Best Blob's Boundary
                    Rect brect = best_blob.getRect();
                    rectangle(output_mat, Rect(brect.x - 10, brect.y - 10, brect.width + 20, brect.height + 20), Scalar(0, 0, 255), 2);
                    putText(output_mat, "#: " + to_string(best_blob.getBlobPixels().size()) + " S:" + to_string(best_blob.getScore()), Point(brect.x, brect.y - 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);

                    //Scalar average_bgr = best_blob.getAverageBGR(0);
                    //string average_bgr_str = to_string((int)average_bgr[0]) + "\n" + to_string((int)average_bgr[1]) + "\n" + to_string((int)average_bgr[2]) + "\n";
                    //cout << average_bgr_str << "";
                    //average_bgr = best_blob.getAverageBGR(1);
                    //average_bgr_str = to_string((int)average_bgr[0]) + "\n" + to_string((int)average_bgr[1]) + "\n" + to_string((int)average_bgr[2]) + "\n";
                    //cout << average_bgr_str << "";
                    //average_bgr = best_blob.getAverageBGR(2);
                    //average_bgr_str = to_string((int)average_bgr[0]) + "\n" + to_string((int)average_bgr[1]) + "\n" + to_string((int)average_bgr[2]) + "\n";
                    //cout << average_bgr_str << "\n";

                    // Display info at bottom of screen
                    Scalar average_bgr = best_blob.getAverageBGR(0);
                    string average_bgr_str = "#Blobs: " + to_string(new_frame.getBlobs().size()) + 
                                             " BGR: ("  + to_string((int)average_bgr[0])         + 
                                                   ", " + to_string((int)average_bgr[1])         + 
                                                   ", " + to_string((int)average_bgr[2])         + ")";
                    putText(output_mat, average_bgr_str, Point(10, MAX_ROW - 10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);

                    // Update Pong Paddle
                    pong.updatePaddle(brect.y);
                }
            }

            // Display Pong
            if (pong.isRunning())
            {
                pong.update();
                rectangle(output_mat, pong.getBall(), Scalar(BALL_BLUE, BALL_GREEN, BALL_RED), 2);
                rectangle(output_mat, pong.getPaddle(), Scalar(PADDLE_BLUE, PADDLE_GREEN, PADDLE_RED), 2);
                if (pong.isLost())
                {
                    putText(output_mat, "You Lost!", Point(MAX_COL / 2 - 50, MAX_ROW / 2 - 10), FONT_HERSHEY_SIMPLEX, 1, Scalar(50, 50, 255), 2);
                }
            }

            Mat frame_mat = new_frame.getMat(1, 1);            // Get OpenCV binary image 
            Mat resized_orig;                                  // Image to write resized output_image
            resize(output_mat, resized_orig, Size(1040, 780)); // Actually resize output_image

            imshow("tuning", frame_mat);
            imshow("output", resized_orig);

            cont = processInputs(output_mat);
        }
    }
    catch (int e){
        cout << "An Exception occurred. Exception #" << e << "\n";
    }
    cout << "Goodbye!\n";

    // Camera will be deinitialized automatically in VideoCapture destructor
    return 0;
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
        unsigned int step = img.step;    // Full row width in bytes (so width * 8?)
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
    }
    if (key == 'q')
    {
        return -1;
    }
    return 1;
}

void on_adjust(int, void* ) {}
