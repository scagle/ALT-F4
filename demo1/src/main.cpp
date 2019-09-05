/*! TODO:
 *  - Figure out exactly how opencv stores Mat.data
 *  - Figure out how to lower camera's resolution
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

void printStats(VideoCapture, Mat);

const string capture_name = "capture.png"; // Name of yanked original image

int main(int, char**)
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
    // Variable Declaration
    Mat orig, hsv;       // Original and hue/sat/val matrixes
    namedWindow("original", WINDOW_AUTOSIZE); // Declare window to draw into
    deque<Frame> frames(FRAME_FIFO_SIZE); // FIFO queue to store frames in

    try { 
        // Frame-Size Dependant Variables
        cap >> orig; // get a frame from camera, so we can grab size
        printStats(cap, orig);
        int channels = orig.channels();
        int cols = orig.cols;
        int width = orig.cols;
        int rows = orig.rows;
        int height = orig.rows;
        vector<unsigned char> values(width * height * channels);  // initialize 'values' to correct size

        while (1)
        {
            //// Capture Video
            cap >> orig; // get a frame from camera
            //cvtColor(orig, hsv, COLOR_BGR2HSV); // convert to hsv
            values.assign(orig.data, orig.data + orig.total() * channels); // copy opencv 'Mat' as single vector

            //// Manipulate FIFO queue
            vector<unsigned int> sums = {0, 0, 0};
            frames.pop_back();
            frames.push_front(Frame(values, width, height));
            unsigned int num = 0;

            //// Perform statistics
            // Test
            for (int i = 0; i < FRAME_FIFO_SIZE; i++)
            {
                if (!frames[i].isInitialized())
                {
                    cout << "frames[" << i << "] is uninitialized!\n";
                    break;
                }
                vector<unsigned char> data = frames[i].getValues();
                for (int row = 0; row < orig.rows; row++)
                {
                    for (int column = 0; column < orig.cols; column++)
                    {
                        float b = data[(channels * orig.cols) * row + (column * channels) + 0];
                        float g = data[(channels * orig.cols) * row + (column * channels) + 1];
                        float r = data[(channels * orig.cols) * row + (column * channels) + 2];
                        sums[0] += b;
                        sums[1] += g;
                        sums[2] += r;
                        num += 1;
                    }
                }
                cout << "frame[" << i << "] average bgr is: (" << sums[0] / num << ", " << sums[1] / num << ", " << sums[2] / num << ")\n";
                sums = {0, 0, 0};
                num = 0;
            }
            cout << "*****************************************************************\n";
            
            imshow("original", orig);

            /************************************************************************************
            
            // For segmenting the image in RGB format.
            inRange(hsv, cv::Scalar(hsv_r[0][0], hsv_r[0][1], hsv_r[0][2]), cv::Scalar(hsv_r[1][0], hsv_r[1][1], hsv_r[1][2]), red_thres);
            inRange(hsv, cv::Scalar(hsv_g[0][0], hsv_g[0][1], hsv_g[0][2]), cv::Scalar(hsv_g[1][0], hsv_g[1][1], hsv_g[1][2]), green_thres);

            // Find contours of the Images
            vector<vector<Point> > contours_r, contours_g;
            vector<Vec4i> hierarchy_r, hierarchy_g;
            findContours(red_thres,   contours_r, hierarchy_r, RETR_TREE, CHAIN_APPROX_SIMPLE);
            findContours(green_thres, contours_g, hierarchy_g, RETR_TREE, CHAIN_APPROX_SIMPLE);

            // Filter by Expected Area (if contours exist, otherwise ignore)
            if (contours_r.size() != 0)
            {
                double expected_area = 10;        // Set our expected area of the laser (varies by distance, but w/e)
                double closest_area_r = 10000;    // Start at some ridiculously high number (and work our way down)
                vector<Point> closest_contour_r;  // Will be our location of red dot

                // Filter Red Laser contours
                for (int i = 0; i < contours_r.size(); i++)
                {
                    double area = contourArea(contours_r[i]);       // Get Area of Contour
                    if (abs(area - expected_area) < closest_area_r) // If the Magnitude is less than previous smallest contour ...
                    {
                        closest_area_r = area;              // We've found our new closest area!
                        closest_contour_r = contours_r[i];  // We've found our new closest contour!
                    }

                }
                // Find momemts of red and green lasers
                Moments m_r = moments(closest_contour_r, false);
                Point p_r(m_r.m10 / m_r.m00, m_r.m01 / m_r.m00);
                circle(orig, p_r, 10, Scalar(0, 0, 128),  3);
            }
            // Filter by Expected Area of Green Laser (if contours exist, otherwise ignore)
            if (contours_g.size() != 0)
            {
                double expected_area = 10;        // Set our expected area of the laser (varies by distance, but w/e)
                double closest_area_g = 10000;    // Start at some ridiculously high number (and work our way down)
                vector<Point> closest_contour_g;  // Will be our location of green dot

                // Filter Green Laser contours
                for (int i = 0; i < contours_g.size(); i++)
                {
                    double area = contourArea(contours_g[i]);       // Get Area of Contour
                    if (abs(area - expected_area) < closest_area_g) // If the Magnitude is less than previous smallest contour ...
                    {
                        closest_area_g = area;              // We've found our new closest area!
                        closest_contour_g = contours_g[i];  // We've found our new closest contour!
                    }

                }
                // Find momemts of red and green lasers
                Moments m_g = moments(closest_contour_g, false);
                Point p_g(m_g.m10 / m_g.m00, m_g.m01 / m_g.m00);
                circle(orig, p_g, 10, Scalar(0, 128, 0),  3);
            }

            // Show the images
            imshow("original", orig);
            imshow("red", red_thres);
            imshow("green", green_thres);

            ************************************************************************************/
            // Delay, and Handle character inputs
            unsigned char key = waitKey(1);
            switch (key)
            {
                case 'y':  // (Yank) Capture a still of original frame, save it to file, and display it on another window
                    {
                        Mat captured_image; // Original, monochrome (grayscale), and hue/sat/val matrixes
                        imwrite(capture_name, hsv);
                        captured_image = imread(capture_name, 1);
                        imshow("saved", hsv);
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
                break;
        }
    }
    catch (int e){
        cout << "An Exception occurred. Exception #" << e << "\n";
    }
    cout << "Goodbye!\n";

    // Camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

void printStats(VideoCapture cap, Mat img)
{
    if (DEBUG)
    {
        unsigned int width = img.cols;
        unsigned int height = img.rows;  
        unsigned int step = img.step;    // Full row width in bytes (so width * 8?)
        unsigned int channels = img.channels(); // How many channels does the images have (rgb = 3)
        cout << "width: "       << width                          << "\n";
        cout << "height: "      << height                         << "\n";
        cout << "step: "        << step                           << "\n";
        cout << "step/8: "      << step/8                         << "\n";
        cout << "channels: "    << channels                       << "\n";
        cout << "framerate: "   << cap.get(CAP_PROP_FPS)          << "\n";
        cout << "framewidth: "  << cap.get(CAP_PROP_FRAME_WIDTH)  << "\n";
        cout << "frameheight: " << cap.get(CAP_PROP_FRAME_HEIGHT) << "\n";
        cout << "buffersize: "  << cap.get(CAP_PROP_BUFFERSIZE)   << "\n";
    }
}

