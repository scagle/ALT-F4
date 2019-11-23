#pragma once
#include <sstream>

// Would use unsigned char, but OpenCV needs int pointers for trackbars
struct Color
{
    int b;
    int g;
    int r;

    // Return a pretty presentable version of color (human readable)
    std::string getPrettyString()
    {
        std::ostringstream ss;
        ss << "{ " << b << ", " << g << ", " << r << " }";
        return ss.str();
    }

    // Return a comma separated version of color (to file)
    std::string getStructuredString()
    {
        std::ostringstream ss;
        ss << b << "," << g << "," << r;
        return ss.str();
    }
};
