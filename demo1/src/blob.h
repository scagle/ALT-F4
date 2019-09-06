#ifndef __BLOB_H_INCLUDED__
#define __BLOB_H_INCLUDED__

#include <vector>
#include "pixel.h"

class Blob
{
    unsigned int bound[4];            // min_row, max_row, min_col, max_col
    unsigned int box[4];              // upperleft row, upperleft col, width, height
    std::vector< Pixel > pixels;      // pixels in blob 
    std::vector< Pixel > edge_pixels; // pixels on perimeter of blob 
    Pixel center_pixel[4];            // pixel in center-ish (not exact but close)
    unsigned char initialized = 0;    // boolean to check if initialized

public:
    // Constructors
    Blob();

    // Methods
    void updateBound(/* insert parameters here */);
    unsigned char isInitialized();
    Pixel   operator [](unsigned char i) const { return pixels[i]; }
    Pixel & operator [](unsigned char i)       { return pixels[i]; }

private:
    void update(); // Called by updateBound() at the end

};

#endif
