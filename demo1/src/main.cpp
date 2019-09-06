/*! TODO:
 *  - Figure out how to operator overload Frame class []
 *  - Figure out how to incorporate exponential moving average to find flashies
 *  - Figure out if we need to cluster groups together (to optimize)
 *  - Figure out how to multithread the frame gathering to seperate it from processing (maybe cores?)
 */
// Webcam Tests
//  Testing how to get input from a webcam using OpenCV

#define DEBUG            1     // Choose whether to display certain messages (might need to make a log class in future)
#define CAMERA_INDEX     2     // What camera you want to use. Identify with: "v4l2-ctl --list-devices"
#define FRAME_FIFO_SIZE 10     // How many frames do we want in our buffer? (frequency analysis)

#include <iostream>
#include <deque>
#include <vector>
#include "frame.h"
#include "pixel.h"

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

using namespace cv;
using namespace std;

VideoCapture initializeVideo();
void printStats(VideoCapture, Mat);
int processInputs(Mat);

const string capture_name = "capture.png"; // 'yanked' photo's name in filesystem
deque<Frame>            frames(FRAME_FIFO_SIZE); // FIFO queue to store frames in

int main(int, char**)
{
    VideoCapture cap = initializeVideo();
    // Variable Declaration
    Mat orig, hsv;       // Original and hue/sat/val matrixes
    namedWindow("original", WINDOW_AUTOSIZE); // Window to show original images
    namedWindow("modified", WINDOW_AUTOSIZE); // Window to show modified images

    try { 
        // Frame-Size Dependant Variables
        cap >> orig; // get a frame from camera, so we can grab size
        printStats(cap, orig);

        int channels = orig.channels();
        int cols     = orig.cols;
        int width    = orig.cols;
        int rows     = orig.rows;
        int height   = orig.rows;
        vector< unsigned char > frame_data(width * height * channels);  // initialize 'frame_data' to correct size

        int cont = 1; // continue variable 
        while (cont == 1)
        {
            //// Capture Video
            cap >> orig; // get a frame from camera
            //cvtColor(orig, hsv, COLOR_BGR2HSV); // convert to hsv
            frame_data.assign(orig.data, orig.data + orig.total() * channels); // copy opencv 'Mat' as single vector

            //// Manipulate FIFO queue
            frames.pop_back();
            frames.push_front(Frame(frame_data, width, height, channels));
            
            
            Mat frame_mat = frames.front().getMat();
                
            imshow("modified", frame_mat);
            imshow("original", orig);

            cont = processInputs(frame_mat);
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
    VideoCapture cap(2);    // open the camera located at /dev/videoX
    if (!cap.isOpened())    // check if we succeeded
    {
        cout << "Camera can't be opened, exiting!\n";
        return -1;
    }
    // Setup VideoCapture settings (if not supported)
    cout << "Setting up Camera settings..." << "\r";
    cout.flush(); // actually print out (output is normally buffered)
    cap.set(CAP_PROP_FPS, 30)           ; // 30 seems to be maximum. (may hang program depending on cam)
    cap.set(CAP_PROP_FRAME_WIDTH, 320)  ; // Lowest possible 4:3 aspect ratio
    cap.set(CAP_PROP_FRAME_HEIGHT, 240) ; // Lowest possible 4:3 aspect ratio
    cap.set(CAP_PROP_BUFFERSIZE, 1)     ; // Reduce internal frame buffer size (we have our own)
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
        cout << "\tcamera width  / rows: " << cap.get(CAP_PROP_FRAME_WIDTH)  << "\n";
        cout << "\tcamera height / cols: " << cap.get(CAP_PROP_FRAME_HEIGHT) << "\n";
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
    }
    if (key == 'q')
    {
        return -1;
    }
    return 1;
}
