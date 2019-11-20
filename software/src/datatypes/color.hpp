#pragma once
#include <sstream>

// Would use unsigned char, but OpenCV needs int pointers for trackbars
struct Color
{
    int b;
    int g;
    int r;

    std::string str()
    {
        std::ostringstream ss;
        ss << "{ " << b << ", " << g << ", " << r << " }";
        return ss.str();
    }
};
