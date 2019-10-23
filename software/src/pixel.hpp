#ifndef __PIXEL_H_INCLUDED__
#define __PIXEL_H_INCLUDED__

#include <vector>
#include <array>

class Pixel
{
    unsigned int row, col;                        // position of pixel
    std::vector< unsigned char > bgr = {0, 0, 0}; // blue green red values of pixel
    unsigned char initialized = 0;                // boolean to check if initialized
public:
    // Constructors
    Pixel();
    Pixel(unsigned int, unsigned int, unsigned char, unsigned char, unsigned char);

    // Methods
    void init(unsigned int, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char);
    unsigned char isInitialized();
    unsigned int getRow() const;
    unsigned int getCol() const;
    unsigned char   operator [](unsigned char i) const { return bgr[i]; }
    unsigned char & operator [](unsigned char i)       { return bgr[i]; }

    // vectors copy by value, so address matching will not work
    bool operator ==(const Pixel &p) { return (this->row == p.getRow() && this->col == p.getCol()); } 
};

#endif
