#include <iostream>
#include <vector>

#include "blob.h"

/**********************************************************************************/
/****                             Constructors                                 ****/
/**********************************************************************************/
Blob::Blob() 
{
    init(std::vector< Pixel >(), std::vector< Pixel >(), 0, 0, 0, 0, 0);
}

Blob::Blob(std::vector< Pixel > blob_pixels, std::vector< Pixel > edge_pixels, 
                unsigned int min_row, unsigned int min_col, unsigned int max_row, unsigned int max_col)
{ 
    init(blob_pixels, edge_pixels, min_row, min_col, max_row, max_col, 1);
}

/**********************************************************************************/
/****                                Methods                                   ****/
/**********************************************************************************/

void Blob::init(std::vector< Pixel > blob_pixels, std::vector< Pixel > edge_pixels, 
                unsigned int min_row, unsigned int min_col, unsigned int max_row, unsigned int max_col,
                unsigned char initialized)
{
     this->blob_pixels = blob_pixels;
     this->edge_pixels = edge_pixels;
     this->rect = cv::Rect(min_col, min_row, max_col - min_col, max_row - min_row);
     this->initialized = initialized;
}

std::vector< Pixel > Blob::getBlobPixels()
{
    return this->blob_pixels;
}

std::vector< Pixel > Blob::getEdgePixels()
{
    return this->edge_pixels;
}

cv::Rect Blob::getRect()
{
    return this->rect;
}

