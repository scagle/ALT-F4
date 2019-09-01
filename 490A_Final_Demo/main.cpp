// Webcam Tests
//  Testing how to get input from a webcam using OpenCV

#include <iostream>
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

    // WE GOT A CAMERA!!!
    try { 
        // Declare some variables we'll need
        namedWindow("original", WINDOW_AUTOSIZE); // Declare window to draw into
        namedWindow("red",      WINDOW_AUTOSIZE); // Declare window to draw into
        namedWindow("green",    WINDOW_AUTOSIZE); // Declare window to draw into
        namedWindow("saved",    WINDOW_AUTOSIZE); // Declare window to draw into
        Mat orig, hsv;       // Original and hue/sat/val matrixes
        Mat red_thres, green_thres;  // Threshold Image for red/green lasers
        int hsv_r[2][3] = { {153, 38, 151}, {255, 255, 255} };  // Red_Laser   {lows{h, s, v} , highs{h, s, v}}
        int hsv_g[2][3] = { {33, 91, 191}, {103, 255, 255} };  // Green_Laser {lows{h, s, v} , highs{h, s, v}}

        // HSV is good for eliminating shadows/light effects
        while (1)
        {
            // Capture Video
            cap >> orig; // get a new frame from camera

            // Convert BGR to HSV format
            // OpenCV Version >= 3.0 
            cvtColor(orig, hsv, COLOR_BGR2GRAY);
            // OpenCV Version < 3.0 
            // cvtColor(orig, hsv, CV_BGR2HSV);

            // Create Trackbars for tweaking variables
            /*! TODO: Include Green Laser, or just change the tables as needed
             *  \todo Include Green Laser, or just change the tables as needed
             *  */
            createTrackbar( "h_l:", "original", &hsv_g[0][0], 255, on_adjust );
            createTrackbar( "s_l:", "original", &hsv_g[0][1], 255, on_adjust );
            createTrackbar( "v_l:", "original", &hsv_g[0][2], 255, on_adjust );
            createTrackbar( "h_h:", "original", &hsv_g[1][0], 255, on_adjust );
            createTrackbar( "s_h:", "original", &hsv_g[1][1], 255, on_adjust );
            createTrackbar( "v_h:", "original", &hsv_g[1][2], 255, on_adjust );
             
            // For segmenting the image in RGB format.
            inRange(hsv, cv::Scalar(hsv_r[0][0], hsv_r[0][1], hsv_r[0][2]), cv::Scalar(hsv_r[1][0], hsv_r[1][1], hsv_r[1][2]), red_thres);
            inRange(hsv, cv::Scalar(hsv_g[0][0], hsv_g[0][1], hsv_g[0][2]), cv::Scalar(hsv_g[1][0], hsv_g[1][1], hsv_g[1][2]), green_thres);

            // Find contours of the Images
            vector<vector<Point> > contours_r, contours_g;
            vector<Vec4i> hierarchy_r, hierarchy_g;
            findContours(red_thres,   contours_r, hierarchy_r, RETR_TREE, CHAIN_APPROX_SIMPLE);
            findContours(green_thres, contours_g, hierarchy_g, RETR_TREE, CHAIN_APPROX_SIMPLE);

            // Filter by Expected Area (if contours exist, otherwise ignore)
            /*! TODO: PUT INTO FUNCTION TO CLEAN UP MAIN
             *  \todo PUT INTO FUNCTION TO CLEAN UP MAIN
             */
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
            /*! TODO: PUT INTO FUNCTION TO CLEAN UP MAIN
             *  \todo PUT INTO FUNCTION TO CLEAN UP MAIN
             */
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

            // Delay, and Handle character inputs
            /*! TODO: PUT INTO FUNCTION TO CLEAN UP MAIN
             *  \todo PUT INTO FUNCTION TO CLEAN UP MAIN
             */
            unsigned char key = waitKey(100);
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
                            cout << "ERROR: Screen Shot failed! (do you have 'scrot' installed?)" << "\n";
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