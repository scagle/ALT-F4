// Webcam Tests
//  Testing how to get input from a webcam using OpenCV

#include <iostream>
#if CV_MAJOR_VERISON < 4   // Tested using OpenCV 2.4.9 (through apt-get)
    #include <opencv2/video/video.hpp>
    #include <opencv2/imgproc/imgproc.hpp>
    #include <opencv2/highgui/highgui.hpp>
    #include <opencv2/core/core.hpp>
#else                      // For newer versions
//  #include <opencv2/opencv.hpp>  // Includes EVERYTHING
    #include <opencv2/videoio.hpp>
    #include <opencv2/imgproc.hpp>
    #include <opencv2/highgui.hpp>
    #include <opencv2/core.hpp>
#endif

using namespace cv;
using namespace std;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
    {
        cout << "Camera can't be opened, exiting!\n";
        return -1;
    }

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
        unsigned char* data = (unsigned char *)small_frame.data; 
        cout << data << "\n";
        
        // Show Image
        imshow("test", small_frame);
        waitKey(0);

    } 
    catch (int e){
        cout << "An Exception occurred. Exception #" << e << "\n";
    }
    cout << "Goodbye!\n";

    // Camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
