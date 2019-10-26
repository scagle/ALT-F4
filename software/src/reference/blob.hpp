#ifndef __BLOB_H_INCLUDED__
#define __BLOB_H_INCLUDED__

#include <opencv2/core.hpp> // For rect
#include <vector>
#include "pixel.hpp"

/**********************************************************************************/
/****                              Structures                                  ****/
/**********************************************************************************/
class Blob
{
    cv::Rect rect;            
    std::vector< Pixel > blob_pixels; // pixels in blob 
    std::vector< Pixel > edge_pixels; // pixels on perimeter of blob 
    std::vector< Pixel > core_pixels; // pixels inside blob (blob_pixels - edge_pixels)
    Pixel center_pixel[4];            // pixel in center-ish (not exact but close)
    cv::Scalar average_bgr_blob;
    cv::Scalar average_bgr_edge;
    cv::Scalar average_bgr_core;
    int score = 0;
    unsigned char initialized = 0;    // boolean to check if initialized
public:
    // Constructors
    Blob();
    Blob(std::vector< Pixel >, std::vector< Pixel >, std::vector< Pixel >, unsigned int, unsigned int, unsigned int, unsigned int);

    // Methods
    void init(std::vector< Pixel >, std::vector< Pixel >, std::vector< Pixel >, unsigned int, unsigned int, unsigned int, unsigned int, unsigned char);
    void updateBound(/* insert parameters here */);
    unsigned char isInitialized();

    // Gets
    std::vector< Pixel > getBlobPixels();
    std::vector< Pixel > getEdgePixels();
    std::vector< Pixel > getCorePixels();
    cv::Rect getRect();
    int getScore();
    cv::Scalar getAverageBGR(int);

    // Sets
    void setScore(int);
    void setAverageBGR(cv::Scalar, int);

    // Operator Overloads
    Pixel   operator [](unsigned char i) const { return blob_pixels[i]; }
    Pixel & operator [](unsigned char i)       { return blob_pixels[i]; }

private:
    void update(); // Called by updateBound() at the end

};

#endif
