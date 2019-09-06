#include <opencv2/core.hpp>

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
        std::cout << "*** Warning: trying to return uninitialized 'values' 1D array (frame.cpp -> getValues())\n";
    }
    return values; 
}

std::vector< std::vector< Pixel > >& Frame::getPixels()
{ 
    // Don't return garbage if not initialized
    if (this->initialized == 0)
        std::cout << "*** Warning: trying to return uninitialized 'pixels' 2D array (frame.cpp -> getPixels())\n";
    return pixels; 
}

cv::Mat& Frame::getMat()
{ 
    // Don't return garbage if not initialized
    if (this->initialized == 0)
        std::cout << "*** Warning: trying to return an image of uninitialized frame (frame.cpp -> getMat())\n";
    else if (this->rows * this->cols * this->channels != 230400)
        std::cout << "*** Warning: Camera has Aspect Ratio " << this->cols << "x" << this-rows << " (expecting 320x240) (Frame -> getMat())\n";
    for (int row = 0; row < this->rows; row++)
    {
        for (int col = 0; col < this->cols; col++)
        {
            for (int ch = 0; ch < this->channels; ch++)
            {
                unsigned long index = (this->channels * this->cols) * row + (col * this->channels) + ch;
                if (this->initialized)
                    pixels1D[index] = this->pixels[row][col][ch];
                else
                    pixels1D[index] = 0;
            }
        }
    }
    output_frame = cv::Mat(240, 320, CV_8UC3, pixels1D);
    return output_frame; 
}

unsigned char Frame::isInitialized()
{ 
    return this->initialized; 
}
