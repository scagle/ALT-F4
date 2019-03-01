// Webcam Tests
//  Testing how to get input from a webcam using OpenCV

#include <iostream>
//#include <opencv2/opencv.hpp>  // Includes EVERYTHING
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    try {
        Mat edges;
        namedWindow("test", 1);
        // Get Image
        Mat orig_frame;
        cap >> orig_frame; // get a new frame from camera
        // Get Small Image
        Mat small_frame;
        Size size = {orig_frame.cols/4, orig_frame.rows/4};
        resize(orig_frame, small_frame, size);
        
        // Show Image
        imshow("test", small_frame);
        waitKey(0);

    } 
    catch (int e){
        cout << "An Exception occurred. Exception #" << e << "\n";
    }

    // Camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
