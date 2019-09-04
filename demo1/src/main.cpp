// Webcam Tests
//  Testing how to get input from a webcam using OpenCV

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

void on_adjust(int, void* );

const string capture_name = "capture.png"; // Name of yanked original image

int main(int, char**)
{
    VideoCapture cap(0);    // open the camera located at /dev/videoX
    if (!cap.isOpened())    // check if we succeeded
    {
        cout << "Camera can't be opened, exiting!\n";
        return -1;
    }

    // Variable Declaration
    Mat orig, hsv;       // Original and hue/sat/val matrixes
    namedWindow("original", WINDOW_AUTOSIZE); // Declare window to draw into
    deque<Frame> frames(FRAME_FIFO_SIZE); // FIFO queue to store frames in

    try { 
        // Frame-Size Dependant Variables
        cap >> orig; // get a frame from camera, so we can grab size
        unsigned int width = orig.cols;
        unsigned int height = orig.rows;  
        unsigned int step = orig.step;    // Full row width in bytes (so width * 8?)
        unsigned int channels = orig.channels(); // How many channels does the images have (rgb = 3)
        cout << "width: " << width << "\n";
        cout << "height: " << height << "\n";
        cout << "step: " << step << "\n";
        cout << "step/8: " << step/8 << "\n";
        cout << "channels: " << channels << "\n";
        vector<unsigned char> values(width * height);  // initialize 'values' to correct size

        while (1)
        {
            //// Capture Video
            cap >> orig;                        // get a frame from camera
            //cvtColor(orig, hsv, COLOR_BGR2HSV); // convert to hsv
            values.assign(orig.data, orig.data + orig.total()); // copy opencv 'Mat' as single vector

            //// Manipulate FIFO queue
            vector<unsigned int> sums = {0, 0, 0};
            frames.pop_back();
            frames.push_front(Frame(values, width, height));

            //// Perform statistics
            // Average
            int valid_frames = 0;
            for (int i = 0; i < FRAME_FIFO_SIZE; i++)
            {
                if (frames[i].isInitialized())
                {
                    for (int row = 0; row < height; row++)
                    {
                        cout << row << " row\n";
                        for (int col = 0; col < width; col++)
                        {
                            for (int ch = 0; ch < channels; ch++)
                            {
                                unsigned int index = (channels * step * row) + (channels * ch) + ch;
                                sums[ch] += frames[i].getValues()[index];
                                //unsigned char b = frames[i].getValues()[step * row + col    ];
                                //unsigned char g = frames[i].getValues()[step * row + col + 1];
                                //unsigned char r = frames[i].getValues()[step * row + col + 2];
                            }
                        }
                    }
                    //cout << "(" << +frames[i].getValues()[0] << ", ";
                    //cout << +frames[i].getValues()[1] << ", ";
                    //cout << +frames[i].getValues()[2] << ")\n";
                    cout << i << " frame finished\n";
                    valid_frames += 1;
                }
                else 
                {
                    cout << "frames[" << i << "] is uninitialized!\n";
                    break;
                }
                cout << "(" << +sums[0] / valid_frames << ", ";
                cout <<        +sums[1] / valid_frames << ", ";
                cout <<        +sums[2] / valid_frames << ")\n";
            }
            cout << "\n";

            //cout << values[0] << ", " << values[1] << ", " << values[2] << ", " << "\n";
            
            imshow("original", hsv);

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



void on_adjust(int, void* ) 
{ 
    // Gets called everytime bars update
}