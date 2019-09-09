/*! TODO:
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
#include <string>
#include "frame.h"

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
void on_adjust(int, void*);

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

        int bgr_r[2][3] = { {0, 0 , 205}, {182, 182, 255} };  // Red_Laser   {low{b, g, r} , high{b, g, r}}
        int bgr_g[2][3] = { {33 , 91 , 191}, {103, 255, 255} };  // Green_Laser {low{b, g, r} , high{b, g, r}}

        createTrackbar( "b_l:", "original", &bgr_r[0][0], 255, on_adjust );
        createTrackbar( "g_l:", "original", &bgr_r[0][1], 255, on_adjust );
        createTrackbar( "r_l:", "original", &bgr_r[0][2], 255, on_adjust );
        createTrackbar( "b_h:", "original", &bgr_r[1][0], 255, on_adjust );
        createTrackbar( "g_h:", "original", &bgr_r[1][1], 255, on_adjust );
        createTrackbar( "r_h:", "original", &bgr_r[1][2], 255, on_adjust );

        int cont = 1; // continue variable 
        while (cont == 1)
        {
            //// Capture Video
            cap >> orig; // get a frame from camera
            //cvtColor(orig, hsv, COLOR_BGR2HSV); // convert to hsv
            frame_data.assign(orig.data, orig.data + orig.total() * channels); // copy opencv 'Mat' as single vector

            //// Manipulate FIFO queue
            frames.pop_back();
            Frame new_frame = Frame(frame_data, width, height, channels);
            frames.push_front(new_frame);

            //// Blob Detection
            new_frame.inRange(bgr_r[0][0], bgr_r[1][0], bgr_r[0][1], bgr_r[1][1], bgr_r[0][2], bgr_r[1][2]);
            new_frame.findBlobs();

            Mat frame_mat = new_frame.getMat(1, 1);
            //// Blob Selection
            if (new_frame.hasBlobs())
            {
                unsigned int filters = (0 << 0) + (0 << 1) + (1 << 2) + // blob B G R
                                       (0 << 3) + (0 << 4) + (1 << 5) + // edge B G R
                                       (1 << 6) + (1 << 7) + (1 << 8) + // core B G R
                                       (0 << 9)                       ; // size

                unsigned char blob_bgr[] = {174, 162, 223}; // red-white
                unsigned char edge_bgr[] = {180, 180, 255}; // red
                unsigned char core_bgr[] = {255, 255, 255}; // white
                Blob best_blob = new_frame.bestBlob(filters, blob_bgr, edge_bgr, core_bgr);
                if (best_blob.isInitialized())
                {
                    Rect brect = best_blob.getRect();
                    rectangle(orig, Rect(brect.x - 10, brect.y - 10, brect.width + 20, brect.height + 20), Scalar(0, 0, 255), 2);
                    putText(orig, to_string(best_blob.getScore()), Point(brect.x, brect.y - 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);

                    Scalar average_bgr = best_blob.getAverageBGR(0);
                    string average_bgr_str = to_string(new_frame.getBlobs().size()) + " (" + to_string((int)average_bgr[0]) + ", " + to_string((int)average_bgr[1]) + ", " + to_string((int)average_bgr[2]) + ")";
                    //cout << average_bgr_str << "\n";
                    //average_bgr = best_blob.getAverageBGR(1);
                    //average_bgr_str = "(" + to_string((int)average_bgr[0]) + ", " + to_string((int)average_bgr[1]) + ", " + to_string((int)average_bgr[2]) + ")";
                    //cout << average_bgr_str << "\n";
                    //average_bgr = best_blob.getAverageBGR(2);
                    //average_bgr_str = "(" + to_string((int)average_bgr[0]) + ", " + to_string((int)average_bgr[1]) + ", " + to_string((int)average_bgr[2]) + ")";
                    //cout << average_bgr_str << "\n\n";
                    
                    putText(orig, average_bgr_str, Point(10, 230), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);
                    /*! TODO: Make more accurate by changing bgr in bestBlob() to JUST look at red, and not blue/green
                     *  \todo Make more accurate by changing bgr in bestBlob() to JUST look at red, and not blue/green
                     */
                }
            }

            imshow("modified", frame_mat);
            imshow("original", orig);

            cont = processInputs(orig);
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

void on_adjust(int, void* ) {}
