#pragma once
#include <vector>

class Frame
{
    unsigned char **values;         // pixel rgb values represented as a single intensity value (0 - 255)
    unsigned int width, height;     // height and width of frame
    unsigned int time;              // timestamp of when frame was taken
    unsigned char initialized = 0;  // boolean to check if initialized
    public:
        Frame(unsigned char **values, unsigned int, unsigned int);
        unsigned char** getValues(               );
        void            setValues(unsigned char**);
};

