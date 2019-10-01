#include <iostream>
#include "pixel.hpp"

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

unsigned int Pixel::getRow() const
{ 
    return this->row; 
}

unsigned int Pixel::getCol() const
{ 
    return this->col; 
}

unsigned char Pixel::isInitialized()
{ 
    return this->initialized; 
}
