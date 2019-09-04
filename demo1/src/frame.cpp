#include "frame.h"

Frame::Frame(unsigned char **values, unsigned int width, unsigned int height)
{
    this->values = values;
    this->width  = width;
    this->height = height;
    this->initialized = 1;  // Verify that we are indeed initialized
}

unsigned char** Frame::getValues()
{ return values; }

void Frame::setValues(unsigned char**)
{ this->values = values; }
