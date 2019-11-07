#pragma once

// Would use unsigned char, but OpenCV needs int pointers for trackbars
struct Color
{
    int b;
    int g;
    int r;
};
