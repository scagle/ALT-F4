#ifndef __BLOB_H_INCLUDED__
#define __BLOB_H_INCLUDED__

#include <opencv2/core.hpp> // For rect
#include <vector>
#include "pixel.h"

/**********************************************************************************/
/****                              Structures                                  ****/
/**********************************************************************************/
class Blob
{
    cv::Rect rect;            
    std::vector< Pixel > blob_pixels; // pixels in blob 
    std::vector< Pixel > edge_pixels; // pixels on perimeter of blob 
    Pixel center_pixel[4];            // pixel in center-ish (not exact but close)
    unsigned char initialized = 0;    // boolean to check if initialized

public:
    // Constructors
    Blob();
    Blob(std::vector< Pixel >, std::vector< Pixel >, unsigned int, unsigned int, unsigned int, unsigned int);

    // Methods
    void init(std::vector< Pixel >, std::vector< Pixel >, unsigned int, unsigned int, unsigned int, unsigned int, unsigned char);
    void updateBound(/* insert parameters here */);
    unsigned char isInitialized();
    std::vector< Pixel > getBlobPixels();
    std::vector< Pixel > getEdgePixels();
    cv::Rect getRect();
    Pixel   operator [](unsigned char i) const { return blob_pixels[i]; }
    Pixel & operator [](unsigned char i)       { return blob_pixels[i]; }

private:
    void update(); // Called by updateBound() at the end

};

#endif
