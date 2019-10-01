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

typedef struct Pixel
{
    unsigned int r;
    unsigned int b;
    unsigned int g;
} Pixel;

typedef Vec<uchar, 3> Vec3b;


int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
    {
        cout << "Camera can't be opened, exiting!\n";
        return -1;
    }
//    cap.set(CAP_PROP_FRAME_WIDTH,  320);
//    cap.set(CAP_PROP_FRAME_HEIGHT, 240);
    try {
        namedWindow("test", 1);
        namedWindow("test2", 2);
        // Get Image
        Mat frame;
        while (1)
        {
            cap >> frame; // get a new frame from camera
            imshow("test2", frame);
            unsigned int width = frame.cols;
            unsigned int height = frame.rows;

            // Grabbing pixel data from image (formatted as B, G, R)
            unsigned char blues [width][height];
            unsigned char greens[width][height];
            unsigned char reds  [width][height];
            for (int x = 0; x < width; x++)
            {
                for (int y = 0; y < height; y++)
                {
                    // Blue
                    blues[x][y] = frame.data[frame.channels()*(width*y + x) + 0];    
                    
                    // Green
                    greens[x][y] = frame.data[frame.channels()*(width*y + x) + 1];

                    // Red
                    reds[x][y] = frame.data[frame.channels()*(width*y + x) + 2];
                }
            }

            // Applying convolution
            int conv_image[width-2][height-2]; // We will lose border pixels
            const char kernel[3][3] = 
            {
                {  0, -1,  0 },      
                { -1,  4, -1 },
                {  0, -1,  0 },
    //            {  1,  0, -1 },
    //            {  0,  0,  0 },
    //            { -1,  0,  1 },
            };
            int max = 0, min = 0;
            for (int x = 1; x < width-1; x++)
            {
                for (int y = 1; y < height-1; y++)
                {
                    int val = 0;
                    for (int kx = -1; kx <= 1; kx++)
                    {
                        for (int ky = -1; ky <= 1; ky++)
                        {
                            val = val + (kernel[kx+1][ky+1] * reds[x-kx][y-ky]);
                        }
                    }
                    conv_image[x-1][y-1] = val;
                    if (val > max)
                        max = val;
                    else if (val < min)
                        min = val;
                }
            }
            // Normalizing Convolution Matrix 
            unsigned char final_image[width-2][height-2]; // We will lose border pixels
            for (int x = 0; x < width-2; x++)
            {
                for (int y = 0; y < height-2; y++)
                {
                    final_image[x][y] = (unsigned char)((double)(conv_image[x][y] - min) * (255 / double((max - min))));
                }
            }

            // Get image from arrays 
            const Mat img(width-2, height-2,  CV_8UC1, final_image);

            // Show Image
            imshow("test", img);
            unsigned char key = waitKey(100);
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
