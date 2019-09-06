#include <iostream>
#include <vector>
#include <array>

#include "frame.h"

/**********************************************************************************/
/****                             Constructors                                 ****/
/**********************************************************************************/
Frame::Frame() 
{
    init(std::vector< unsigned char >(), 0, 0, 0, 0);
}

Frame::Frame(std::vector< unsigned char > values, unsigned int width, unsigned int height, 
             unsigned int channels)
{ 
    init(values, width, height, channels, 1);
}

/**********************************************************************************/
/****                                Methods                                   ****/
/**********************************************************************************/
void Frame::init(std::vector< unsigned char > values, unsigned int width, unsigned int height, 
                 unsigned int channels, unsigned char initialized)
{
    this->values = values; 
    this->width  = width; 
    this->height = height; 
    this->cols   = width; 
    this->rows   = height; 
    this->channels = channels; 
    this->initialized = initialized;  // Verify that we are indeed initialized
    if (this->initialized)
        convertValues();
}

void Frame::convertValues()
{
    this->pixels.resize(rows);
    for (unsigned int row = 0; row < rows; row++)
    {
        this->pixels[row].resize(cols);
        for (unsigned int col = 0; col < cols; col++)
        {
            unsigned char b = this->values[(channels * cols) * row + (col * channels) + 0];
            unsigned char g = this->values[(channels * cols) * row + (col * channels) + 1];
            unsigned char r = this->values[(channels * cols) * row + (col * channels) + 2];
            this->pixels[row][col] = Pixel(row, col, b, g, r);
        }
    }
}

std::vector< unsigned char >& Frame::getValues()
{ 
    // Don't return garbage if not initialized
    if (this->initialized == 0)
    {
        std::cout << "*** Warning: trying to return uninitialized values (frame.cpp -> getValues())\n";
    }
    return values; 
}

std::vector< std::vector< Pixel > >& Frame::getPixels()
{ 
    // Don't return garbage if not initialized
    if (this->initialized == 0)
        std::cout << "*** Warning: trying to return uninitialized pixels (frame.cpp -> getPixels())\n";
    return pixels; 
}

std::vector< unsigned char > Frame::getPixels1D()
{ 
    // Don't return garbage if not initialized
    if (this->initialized == 0)
        std::cout << "*** Warning: trying to return uninitialized pixels (frame.cpp -> getPixels())\n";
    if (this->rows * this->cols * this->channels != 230400)
        std::cout << "*** Warning: Camera has Aspect Ratio " << this->cols << "x" << this-rows << " (expecting 320x240) (Frame -> getPixels1D())\n";
    std::vector< unsigned char > pixels1D(230400, 0); 
    unsigned long count = 0;
    for (int row = 0; row < this->rows; row++)
    {
        for (int col = 0; col < this->cols; col++)
        {
            for (int ch = 0; ch < this->channels; ch++)
            {
                unsigned long index = (this->channels * this->cols) * row + (col * this->channels) + ch;
                pixels1D[index] = this->pixels[row][col][ch];
                if (count != index)
                    std::cout << "@@@ DEBUG: misaligned index (" << count << " != " << index << ")\n";
                //if (count <= 31)
                //    std::cout << "@@@ DEBUG: " << index << ": " << +this->pixels[row][col][ch] << "\n";
                count++;

            }
        }
    }
    return pixels1D; 
}

unsigned char Frame::isInitialized()
{ 
    return this->initialized; 
}
