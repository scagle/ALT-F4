#pragma once

#include "position.hpp"
#include "color.hpp"

struct CoreAnchor
{
    Position anchor;  // Where line is anchored (origin -> anchor)
    Color avg_color;  // Average color of line
    int length;       // length of line
    bool exploded;    // Whether is exploded or not
};
