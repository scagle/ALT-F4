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

typedef Vec<uchar, 3> Vec3b;
const string capture_name = "capture.png";

int main(int, char**)
{
    VideoCapture cap(1); // open the default camera
    if (!cap.isOpened())  // check if we succeeded
    {
        cout << "Camera can't be opened, exiting!\n";
        return -1;
    }
    try {
        namedWindow("original", WINDOW_AUTOSIZE);
        namedWindow("red", WINDOW_AUTOSIZE);
        namedWindow("green", WINDOW_AUTOSIZE);
        namedWindow("saved", WINDOW_AUTOSIZE);
        // Get Image
        Mat orig, mono, hsv; // Original, monochrome (grayscale), and hue/sat/val matrixes
        Mat red_thres;   // Image for red laser
        Mat green_thres; // Image for green laser
        int h_l = 153;   // hue low
        int s_l = 38;    // saturation low
        int v_l = 151;    // value low
        int h_h = 255;   // hue high
        int s_h = 255;   // saturation high
        int v_h = 255;   // value high

        // HSV is good for eliminating shadows/light effects
        while (1)
        {
            cap >> orig; // get a new frame from camera
            cvtColor(orig, hsv, CV_BGR2HSV);
            // For segmenting the image in RGB format.
            inRange(hsv, cv::Scalar(h_l, s_l, v_l), cv::Scalar(h_h, s_h, v_h), red_thres);
            inRange(hsv, cv::Scalar(h_l, s_l, v_l), cv::Scalar(h_h, s_h, v_h), green_thres);
            createTrackbar( "h_l:", "original", &h_l, 255, on_adjust );
            createTrackbar( "s_l:", "original", &s_l, 255, on_adjust );
            createTrackbar( "v_l:", "original", &v_l, 255, on_adjust );
            createTrackbar( "h_h:", "original", &h_h, 255, on_adjust );
            createTrackbar( "s_h:", "original", &s_h, 255, on_adjust );
            createTrackbar( "v_h:", "original", &v_h, 255, on_adjust );
             
            // find moments of the images
            Moments m_r = moments(red_thres, true);
            Point p_r(m_r.m10 / m_r.m00, m_r.m01 / m_r.m00);
            Moments m_g = moments(green_thres, true);
            Point p_g(m_g.m10 / m_g.m00, m_g.m01 / m_g.m00);
             
            //// coordinates of centroid
            // cout<< Mat(p_r)<< endl;
            // cout<< Mat(p_g)<< endl;
             
            // show the image with a point mark at the centroid
            circle(orig, p_r, 5, Scalar(128,0,0), -1);
            circle(orig, p_g, 5, Scalar(128,0,0), -1);

            imshow("original", orig);
            imshow("red", red_thres);
            imshow("green", green_thres);

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
{ }

//            unsigned int width = frame.cols;
//            unsigned int height = frame.rows;
//
//            // Grabbing pixel data from image (formatted as B, G, R)
//            unsigned char blues [width][height];
//            unsigned char greens[width][height];
//            unsigned char reds  [width][height];
//            for (int x = 0; x < width; x++)
//            {
//                for (int y = 0; y < height; y++)
//                {
//                    // Blue
//                    blues[x][y] = frame.data[frame.channels()*(width*y + x) + 0];    
//                    
//                    // Green
//                    greens[x][y] = frame.data[frame.channels()*(width*y + x) + 1];
//
//                    // Red
//                    reds[x][y] = frame.data[frame.channels()*(width*y + x) + 2];
//                }
//            }
//
//            // Applying convolution
//            int conv_image[width-2][height-2]; // We will lose border pixels
//            const int kernel[3][3] = 
//            {
//    //          { -1, -1, -1 },      
//    //          { -1,  8, -1 },
//    //          { -1, -1, -1 },
//                {  0, -1,  0 },      
//                { -1,  4, -1 },
//                {  0, -1,  0 },
//    //            {  1,  0, -1 },
//    //            {  0,  0,  0 },
//    //            { -1,  0,  1 },
//            };
//            int max = 0, min = 0;
//            for (int x = 1; x < width-1; x++)
//            {
//                for (int y = 1; y < height-1; y++)
//                {
//                    int val = 0;
//                    for (int kx = -1; kx <= 1; kx++)
//                    {
//                        for (int ky = -1; ky <= 1; ky++)
//                        {
//                            val = val + (kernel[kx+1][ky+1] * greens[x-kx][y-ky]);
//                        }
//                    }
//                    conv_image[x-1][y-1] = val;
//                    if (val > max)
//                        max = val;
//                    else if (val < min)
//                        min = val;
//                }
//            }
//            // TODO: Figure out transposition 
//            // Normalizing Convolution Matrix 
//            unsigned char final_image[height-2][width-2]; // We will lose border pixels
//            for (int x = 0; x < width-2; x++)
//            {
//                for (int y = 0; y < height-2; y++)
//                {
//                    final_image[y][x] = (unsigned char)((double)(conv_image[x][y] - min) * (255 / double((max - min))));
//                }
//            }
//
//            // Get image from arrays 
//            const Mat img(height-2, width-2, CV_8UC1, final_image);
//
//            // Show Image
//            imshow("test", img);
//            unsigned char key = waitKey(500);
//            if (key == 'q')
//                break;
