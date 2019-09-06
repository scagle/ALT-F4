#include <iostream>
#include "pixel.h"

/**********************************************************************************/
/****                             Constructors                                 ****/
/**********************************************************************************/
Pixel::Pixel() 
{
    init(0, 0, 0, 0, 0, 0);
}

Pixel::Pixel(unsigned int row, unsigned int col, unsigned char b, unsigned char g, unsigned char r)
{ 
    init(row, col, b, g, r, 1);
}

/**********************************************************************************/
/****                                Methods                                   ****/
/**********************************************************************************/
void Pixel::init(unsigned int row, unsigned int col, unsigned char b, unsigned char g, unsigned char r, 
                 unsigned char initialized)
{
    this->row = row; 
    this->col = col; 
    this->bgr[0] = b; 
    this->bgr[1] = g; 
    this->bgr[2] = r; 
    this->initialized = initialized;  // Verify that we are indeed initialized
}

unsigned char Pixel::isInitialized()
{ 
    return this->initialized; 
}
