#pragma once
#include <vector>

class Frame
{
    std::vector<unsigned char> values;  // pixel rgb values represented as a single intensity value (0 - 255)
    unsigned int width, height;         // height and width of frame
    unsigned int time;                  // timestamp of when frame was taken
    unsigned char initialized = 0;      // boolean to check if initialized
public:
    Frame();
    Frame(std::vector<unsigned char>, unsigned int, unsigned int);
    Frame(const Frame&);
    std::vector<unsigned char>& getValues();
    void setValues(std::vector<unsigned char>, unsigned int, unsigned int);
    unsigned char isInitialized();
};

