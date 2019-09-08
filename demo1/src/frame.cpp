#include <opencv2/core.hpp>

#include <iostream>
#include <cstdlib>
#include <vector>
#include <stack>

#include "frame.h"
#include "blob.h"

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
    this->pixels.resize(this->rows);
    for (unsigned int row = 0; row < this->rows; row++)
    {
        this->pixels[row].resize(this->cols);
        for (unsigned int col = 0; col < this->cols; col++)
        {
            unsigned char b = this->values[(channels * cols) * row + (col * channels) + 0];
            unsigned char g = this->values[(channels * cols) * row + (col * channels) + 1];
            unsigned char r = this->values[(channels * cols) * row + (col * channels) + 2];
            this->pixels[row][col] = Pixel(row, col, b, g, r);
        }
    }
}

void Frame::inRange(unsigned char b_l, unsigned char b_h,  // low / high blue 
                    unsigned char g_l, unsigned char g_h,  // low / high green
                    unsigned char r_l, unsigned char r_h)  // low / high red
{
    /*! TODO: 
     *  - Finish blob class
     *  - Find edges as you go along
     */
    
    // Pseudo code
    // NOTE: If this is bottleneck, combine it with the 'convertValues()' function.
    //       this is separated for readability      
    binary_matrix.resize(this->rows);
    for (int row = 0; row < this->rows; row++)
    {
        binary_matrix[row].resize(this->cols);
        for (int col = 0; col < this->cols; col++)
        {
            unsigned char b = this->values[(channels * cols) * row + (col * channels) + 0];
            unsigned char g = this->values[(channels * cols) * row + (col * channels) + 1];
            unsigned char r = this->values[(channels * cols) * row + (col * channels) + 2];

            binary_matrix[row][col] = 0;     // Start out as 0 
            if (this->initialized)           // If this frame is initialized
            {
                if ((b < b_l) || (b > b_h))  // If out of bounds of blue
                    continue;                // Skip
                if ((g < g_l) || (g > g_h))  // If out of bounds of green
                    continue;                // Skip
                if ((r < r_l) || (r > r_h))  // If out of bounds of red
                    continue;                // Skip
                binary_matrix[row][col] = 1; // Otherwise success!
            }
            else
            {
                binary_matrix[row][col] = 0;
            }
        }
    }
}

void Frame::findBlobs()
{
    std::vector< std::vector < unsigned char > > edited_matrix(this->binary_matrix);
    // "edited_matrix": 
    // 0 -> nothing (skip it)
    // 1 -> juicy_pixel (blob it)
    // 2 -> eaten_pixel (dont blob it)
    
    std::stack < Pixel > blob_stack;
    unsigned int blob_count = 0;
    for (int row = 0; row < this->rows; row++)
    {
        for (int col = 0; col < this->cols; col++)
        {
            if (edited_matrix[row][col] == 1)
            {

                std::vector< Pixel > blob_pixels;
                std::vector< Pixel > edge_pixels;
                // BLOB IT!
                blob_stack.push(this->pixels[row][col]); // start the blob stack 
                blob_pixels.push_back(this->pixels[row][col]); // keep track of pixels in blob
                edited_matrix[row][col] = 2;             // eat the pixel
                unsigned int blob_min_row = row;         // keep track of boundaries of blob
                unsigned int blob_min_col = col;         // keep track of boundaries of blob
                unsigned int blob_max_row = row;         // keep track of boundaries of blob 
                unsigned int blob_max_col = col;         // keep track of boundaries of blob
                while (!blob_stack.empty())              // while there's still pixels to eat...
                {
                    // Grab next pixel
                    Pixel next_pixel = blob_stack.top();
                    int p_row = next_pixel.getRow();
                    int p_col = next_pixel.getCol();
                    blob_stack.pop();                    // swallow pixel

                    // Define some constants
                    const unsigned int MIN_ROW =   0;
                    const unsigned int MIN_COL =   0;
                    const unsigned int MAX_ROW = 240;
                    const unsigned int MAX_COL = 320;

                    // Check all 8 neighbors
                    unsigned int neighbor_count = 0;
                    for (int ro = -1; ro <= 1; ro++) // row offset
                    {
                        for (int co = -1; co <= 1; co++) // col offset
                        {
                            if ( !( ro == 0    && co == 0 ) && // if not in center
                                  ( p_row + ro <  MAX_ROW ) &&
                                  ( p_row + ro >= MIN_ROW ) &&
                                  ( p_col + co <  MAX_COL ) &&
                                  ( p_col + co >= MIN_COL ) )
                            {
                                if (edited_matrix[p_row + ro][p_col + co] == 1)
                                {
                                    blob_stack.push(this->pixels[p_row + ro][p_col + co]);
                                    blob_pixels.push_back(this->pixels[p_row + ro][p_col + co]); 
                                    edited_matrix[p_row + ro][p_col + co] = 2;
                                    if ( row + ro > blob_max_row )
                                        blob_max_row = row + ro;
                                    if ( row + ro < blob_min_row )
                                        blob_min_row = row + ro;
                                    if ( col + co > blob_max_col )
                                        blob_max_col = col + co;
                                    if ( col + co < blob_min_col )
                                        blob_min_col = col + co;
                                }
                                if (edited_matrix[p_row + ro][p_col + co] != 0)
                                    neighbor_count++;
                            }
                        }
                    }
                    if ( neighbor_count != 8 )
                        edge_pixels.push_back(this->pixels[p_row][p_col]); // start the blob stack 
                }
                Blob new_blob = Blob(blob_pixels, edge_pixels, blob_min_row, blob_min_col, blob_max_row, blob_max_col);
                this->blobs.push_back(new_blob);
                blob_count++;
            }
        }
    }
}

Blob Frame::bestBlob(unsigned int filters, unsigned char bgr[3])
{
    /* filters: 
     *  bit 0 = Blue Edge:  Score based on closest bgr value of ONLY edge pixels (targetted at 'laser iris effect')
     *  bit 1 = Green Edge: Score based on closest bgr value of ONLY edge pixels (targetted at 'laser iris effect')
     *  bit 2 = Red Edge:   Score based on closest bgr value of ONLY edge pixels (targetted at 'laser iris effect')
     *  bit 3 = Size: Score based on amount of pixels
     */ 

    if (blobs.size() == 0)
    {
        std::cout << "*** Warning: No blobs at all. Returning empty blob (frame.cpp -> bestBlob())\n";
        return Blob();
    }

    // Filter extraction
    bool edge_filter_blue  = filters & 1;
    bool edge_filter_green = filters & 2;
    bool edge_filter_red   = filters & 4;
    bool size_filter       = filters & 8;

    // Weights
    const int BLUE_WEIGHT  = 1;
    const int GREEN_WEIGHT = 1;
    const int RED_WEIGHT   = 2;
    const int SIZE_WEIGHT  = -2;
    const int SCORE_CUTOFF = 180;

    std::vector< int > scores(blobs.size(), 0);
    for (int i = 0; i < this->blobs.size(); i++)
    {
        std::vector< Pixel > blob_pixels = this->blobs[i].getBlobPixels();
        std::vector< Pixel > edge_pixels = this->blobs[i].getEdgePixels();
        if (edge_filter_blue || edge_filter_green || edge_filter_red)
        {
            unsigned int bgr_sums[3] = {0, 0, 0};
            for (int pi = 0; pi < edge_pixels.size(); pi++) // pixel index
            {
                bgr_sums[0] += edge_pixels[pi][0];
                bgr_sums[1] += edge_pixels[pi][1];
                bgr_sums[2] += edge_pixels[pi][2];
            }
            unsigned char avg_b = bgr_sums[0] / edge_pixels.size();
            unsigned char avg_g = bgr_sums[1] / edge_pixels.size();
            unsigned char avg_r = bgr_sums[2] / edge_pixels.size();
            blobs[i].setAverageBGR(cv::Scalar(avg_b, avg_g, avg_r), 1);

            unsigned char diff_b = std::abs((int)(avg_b) - (int)(bgr[0]));
            unsigned char diff_g = std::abs((int)(avg_g) - (int)(bgr[1]));
            unsigned char diff_r = std::abs((int)(avg_r) - (int)(bgr[2]));

            /*! TODO: Probably want to just score based on red / green, not blue.
             *  \todo Probably want to just score based on red / green, not blue.
             */
            if (edge_filter_blue)
                scores[i] += (255 - diff_b) * BLUE_WEIGHT;
            if (edge_filter_green)
                scores[i] += (255 - diff_g) * GREEN_WEIGHT;
            if (edge_filter_red)
                scores[i] += (255 - diff_r) * RED_WEIGHT;
        }
        if (size_filter)
        {
            scores[i] += edge_pixels.size() * SIZE_WEIGHT;
        }
    }
    int best_score = 0;
    Blob best_blob;
    for (int i = 0; i < this->blobs.size(); i++)
    {
        blobs[i].setScore(scores[i]);
        if (scores[i] > best_score && scores[i] >= SCORE_CUTOFF)
        {
            best_score = scores[i];
            best_blob = blobs[i];
        }
    }
    if (best_blob.isInitialized())
        std::cout << "Best score: " << best_score << "\n";

    return best_blob;
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

cv::Mat& Frame::getMat(int channels = 3, int type = 0)
{ 
    /*! TODO: Generalize this (clean up)
     *  \todo Generalize this (clean up)
     */
    // Channels: However many channels you want
    // Type: 0 = this->pixels
    // Type: 1 = this->binary_matrix
    
    // Don't return garbage if not initialized
    if (this->initialized == 0)
        std::cout << "*** Warning: trying to return an image of uninitialized frame (frame.cpp -> getMat())\n";
    else if (this->rows * this->cols * this->channels != 230400)
        std::cout << "*** Warning: Camera has Aspect Ratio " << this->cols << "x" << this-rows << " (expecting 320x240) (Frame -> getMat())\n";
    if (type == 0)
    {
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
    }
    else if (type == 1)
    {
        for (int row = 0; row < this->rows; row++)
        {
            for (int col = 0; col < this->cols; col++)
            {
                unsigned long index = this->cols * row + col;
                if (this->initialized)
                {
                    binary1D[index] = this->binary_matrix[row][col]*255;
                }
                else
                {
                    binary1D[index] = 0;
                }
            }
        }
        output_frame = cv::Mat(240, 320, CV_8UC1, binary1D);
    }
    else
    {
        std::cout << "*** Warning: Invalid type (frame.cpp -> getMat())\n";
    }
    return output_frame; 
}

unsigned char Frame::isInitialized()
{ 
    return this->initialized; 
}

bool Frame::hasBlobs()
{ 
    return (this->blobs.size() > 0); 
}
