#ifndef __FRAME_H_INCLUDED__
#define __FRAME_H_INCLUDED__

#include <opencv2/core.hpp>

#include <vector>
#include "pixel.h"

class Frame
{
    std::vector< unsigned char > values;        // 1D Array of values (b, g, r, b, g, r, etc...) 
    std::vector< std::vector< Pixel > > pixels; // 2D Array of pixels 
    unsigned int width, height;                 // height and width of frame
    unsigned int cols, rows;                    // synonym for height and width of frame
    unsigned int channels;                      // channels
    unsigned int time;                          // timestamp of when frame was taken
    unsigned char initialized = 0;              // boolean to check if initialized

    unsigned char pixels1D[230400];             // Array used to generate Frame
    cv::Mat output_frame;                       // Frame to be returned to main for display
public:
    // Constructors
    Frame();
    Frame(std::vector<unsigned char>, unsigned int, unsigned int, unsigned int);

    // Methods
    void init(std::vector<unsigned char>, unsigned int, unsigned int, unsigned int, unsigned char);
    void convertValues();
    std::vector< unsigned char >& getValues();
    std::vector< std::vector< Pixel > >& getPixels();
    cv::Mat& getMat();
    void setValues(std::vector< unsigned char >, unsigned int, unsigned int);
    unsigned char isInitialized();
};

#endif
