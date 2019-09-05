#include <iostream>
#include "frame.h"

int count = 0;

/**********************************************************************************/
/****                             Constructors                                 ****/
/**********************************************************************************/
Frame::Frame() 
{
    this->width = 0;  // Verify that we are indeed initialized
    this->height = 0;  // Verify that we are indeed initialized
    this->initialized = 0;  // Verify that we are indeed initialized
}

Frame::Frame(std::vector<unsigned char> values, unsigned int width, unsigned int height)
{ 
    this->values = values; 
    this->width  = width; 
    this->height = height; 
    this->initialized = 1;  // Verify that we are indeed initialized
}

Frame::Frame(const Frame& frame)
{ 
    this->values = frame.values; 
    this->width  = frame.width; 
    this->height = frame.height; 
    this->initialized = 1;  // Verify that we are indeed initialized
}

/**********************************************************************************/
/****                                Methods                                   ****/
/**********************************************************************************/
std::vector<unsigned char>& Frame::getValues()
{ 
    // Don't return garbage if not initialized
    if (this->initialized == 0)
    {
        std::cout << "*** Warning: trying to return uninitialized values (frame.cpp -> getValues())\n";
    }
    return values; 
}

void Frame::setValues(std::vector<unsigned char> values, unsigned int width, unsigned int height)
{ 
    this->values = values; 
    this->width  = width; 
    this->height = height; 
    this->initialized = 1;  // Verify that we are indeed initialized
}

unsigned char Frame::isInitialized()
{ return this->initialized; }
