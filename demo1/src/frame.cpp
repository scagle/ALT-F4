#include <opencv2/core.hpp>

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <stack>

#include "global.h"
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
                std::vector< Pixel > core_pixels;
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

                    // Check all 8 neighbors
                    unsigned int neighbor_count = 0;
                    for (int ro = -MAX_MINESWEEP_OFFSET; ro <= MAX_MINESWEEP_OFFSET; ro++) // row offset
                    {
                        for (int co = -MAX_MINESWEEP_OFFSET; co <= MAX_MINESWEEP_OFFSET; co++) // col offset
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
                                {
                                    if ((std::abs(ro) <= 1) && (std::abs(co) <= 1)) // make sure they're IMMEDIATE neighbors (Max of 8)
                                    {
                                        neighbor_count++;
                                    }
                                }
                            }
                        }
                    }
                    if ( neighbor_count < NEIGHBOR_COUNT )
                    {
                        edge_pixels.push_back(this->pixels[p_row][p_col]); // start the blob stack 
                    }
                }
                // Get core pixels (all pixels in blob but not in edges)
                for (int i = 0; i < blob_pixels.size(); i++)
                {
                    // If pixel is in blob, but NOT in edges then ...
                    if (std::find(edge_pixels.begin(), edge_pixels.end(), blob_pixels[i]) == edge_pixels.end())
                    {
                        core_pixels.push_back(blob_pixels[i]);
                    }
                }
                
                Blob new_blob = Blob(blob_pixels, edge_pixels, core_pixels, blob_min_row, blob_min_col, blob_max_row, blob_max_col);
                this->blobs.push_back(new_blob);
                blob_count++;
            }
        }
    }
}

Blob Frame::bestBlob(const unsigned int filters, 
                     const unsigned char bgr_blob[3], const unsigned char bgr_edge[3], const unsigned char bgr_core[3])
{
    /* filters: 
     *  bit 0 = Blob Blue:   Score based on closest bgr value of ONLY edge pixels (targetted at 'laser iris effect')
     *  bit 1 = Blob Green:  Score based on closest bgr value of ONLY edge pixels (targetted at 'laser iris effect')
     *  bit 2 = Blob Red:    Score based on closest bgr value of ONLY edge pixels (targetted at 'laser iris effect')
     *  bit 3 = Edge Blue:   Score based on closest bgr value of ONLY edge pixels (targetted at 'laser iris effect')
     *  bit 4 = Edge Green:  Score based on closest bgr value of ONLY edge pixels (targetted at 'laser iris effect')
     *  bit 5 = Edge Red:    Score based on closest bgr value of ONLY edge pixels (targetted at 'laser iris effect')
     *  bit 6 = Core Blue:   Score based on closest bgr value of ONLY edge pixels (targetted at 'laser iris effect')
     *  bit 7 = Core Green:  Score based on closest bgr value of ONLY edge pixels (targetted at 'laser iris effect')
     *  bit 8 = Core Red:    Score based on closest bgr value of ONLY edge pixels (targetted at 'laser iris effect')
     *  bit 9 = Size:        Score based on amount of pixels
     */ 

    if (blobs.size() == 0)
    {
        std::cout << "*** Warning: No blobs at all. Returning empty blob (frame.cpp -> bestBlob())\n";
        return Blob();
    }

    // Filter extraction
    bool blob_filter_blue  = filters & (1 << 0);
    bool blob_filter_green = filters & (1 << 1);
    bool blob_filter_red   = filters & (1 << 2);
    bool edge_filter_blue  = filters & (1 << 3);
    bool edge_filter_green = filters & (1 << 4);
    bool edge_filter_red   = filters & (1 << 5);
    bool core_filter_blue  = filters & (1 << 6);
    bool core_filter_green = filters & (1 << 7);
    bool core_filter_red   = filters & (1 << 8);
    bool size_filter       = filters & (1 << 9);

    std::vector< int > scores(blobs.size(), 0);
    for (int i = 0; i < this->blobs.size(); i++)
    {
        std::vector< Pixel > blob_pixels = this->blobs[i].getBlobPixels();
        std::vector< Pixel > edge_pixels = this->blobs[i].getEdgePixels();
        std::vector< Pixel > core_pixels = this->blobs[i].getCorePixels();
        cv::Scalar bgr_blob_avg = {0, 0, 0};
        cv::Scalar bgr_edge_avg = {0, 0, 0};
        cv::Scalar bgr_core_avg = {0, 0, 0};
        cv::Scalar bgr_blob_diffs = {0, 0, 0};
        cv::Scalar bgr_edge_diffs = {0, 0, 0};
        cv::Scalar bgr_core_diffs = {0, 0, 0};

        if ((blob_filter_blue || blob_filter_green || blob_filter_red) && blob_pixels.size() > 0)
        {
            unsigned int bgr_blob_sums[3] = {0, 0, 0};
            for (int pi = 0; pi < blob_pixels.size(); pi++) // pixel index
            {
                for (int ch = 0; ch < this->channels; ch++)
                {
                    bgr_blob_sums[ch] += blob_pixels[pi][ch];
                }
            }
            for (int ch = 0; ch < this->channels; ch++)
            {
                bgr_blob_avg[ch] = bgr_blob_sums[ch] / blob_pixels.size();
                bgr_blob_diffs[ch] = std::abs((int)(bgr_blob_avg[ch]) - (int)(bgr_blob[ch]));
            }
            blobs[i].setAverageBGR(bgr_blob_avg, 0);

            if (blob_filter_blue)
            {
                scores[i] += (255 - bgr_blob_diffs[0]) * BLOB_BLUE_WEIGHT;
            }
            if (blob_filter_green)
            {
                scores[i] += (255 - bgr_blob_diffs[1]) * BLOB_GREEN_WEIGHT;
            }
            if (blob_filter_red)
            {
                scores[i] += (255 - bgr_blob_diffs[2]) * BLOB_RED_WEIGHT;
            }

        }
        if ((edge_filter_blue || edge_filter_green || edge_filter_red) && edge_pixels.size() > 0)
        {
            unsigned int bgr_edge_sums[3] = {0, 0, 0};
            for (int pi = 0; pi < edge_pixels.size(); pi++) // pixel index
            {
                for (int ch = 0; ch < this->channels; ch++)
                {
                    bgr_edge_sums[ch] += edge_pixels[pi][ch];
                }
            }
            for (int ch = 0; ch < this->channels; ch++)
            {
                bgr_edge_avg[ch] = bgr_edge_sums[ch] / edge_pixels.size();
                bgr_edge_diffs[ch] = std::abs((int)(bgr_edge_avg[ch]) - (int)(bgr_edge[ch]));
            }
            blobs[i].setAverageBGR(bgr_edge_avg, 1);

            if (edge_filter_blue)
            {
                scores[i] += (255 - bgr_edge_diffs[0]) * EDGE_BLUE_WEIGHT;
            }
            if (edge_filter_green)
            {
                scores[i] += (255 - bgr_edge_diffs[1]) * EDGE_GREEN_WEIGHT;
            }
            if (edge_filter_red)
            {
                scores[i] += (255 - bgr_edge_diffs[2]) * EDGE_RED_WEIGHT;
            }

        }

        if ((core_filter_blue || core_filter_green || core_filter_red) && core_pixels.size() > 0)
        {
             
            unsigned int bgr_core_sums[3] = {0, 0, 0};
            for (int pi = 0; pi < core_pixels.size(); pi++) // pixel index
            {
                for (int ch = 0; ch < this->channels; ch++)
                {
                    bgr_core_sums[ch] += core_pixels[pi][ch];
                }
            }
            for (int ch = 0; ch < this->channels; ch++)
            {
                bgr_core_avg[ch] = bgr_core_sums[ch] / core_pixels.size();
                bgr_core_diffs[ch] = std::abs((int)(bgr_core_avg[ch]) - (int)(bgr_core[ch]));
            }
            blobs[i].setAverageBGR(bgr_core_avg, 2);

            if (core_filter_blue)
            {
                scores[i] += (255 - bgr_core_diffs[0]) * CORE_BLUE_WEIGHT;
            }
            if (core_filter_green)
            {
                scores[i] += (255 - bgr_core_diffs[1]) * CORE_GREEN_WEIGHT;
            }
            if (core_filter_red)
            {
                scores[i] += (255 - bgr_core_diffs[2]) * CORE_RED_WEIGHT;
            }

        }
        if (size_filter)
        {
            scores[i] += (SIZE_IDEAL - blob_pixels.size()) * -SIZE_WEIGHT;
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
//    if (best_blob.isInitialized())
//        std::cout << "Best score: " << best_score << "\n";

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
    else if (this->rows * this->cols * this->channels != MAX_COL * MAX_ROW * this->channels)
        std::cout << "*** Warning: Camera has Aspect Ratio " << this->cols << "x" << this-rows << " (expecting " << MAX_COL << "x" << MAX_ROW << ") (Frame -> getMat())\n";
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
        output_frame = cv::Mat(MAX_ROW, MAX_COL, CV_8UC3, pixels1D);
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
        output_frame = cv::Mat(MAX_ROW, MAX_COL, CV_8UC1, binary1D);
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

std::vector< Blob > Frame::getBlobs()
{
    return this->blobs;
}

bool Frame::hasBlobs()
{ 
    return (this->blobs.size() > 0); 
}
