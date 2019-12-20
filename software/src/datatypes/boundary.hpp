#pragma once

#include <stdio.h>

namespace altf4
{
    // Mostly for checking boundaries in blobs and core
    // Differs from SDL_Rect or Rect, because it prefers raw coordinates over width/height
    struct Boundary
    {
        int la; // Low A   (Could be X or could be Row)
        int lb; // Low B   (Could be Y or could be Col)
        int ha; // High A  (Could be X or could be Row)
        int hb; // High B  (Could be Y or could be Col)

        void printPretty()
        {
            printf("*********************\n\tArea: %d\n\tX/C: [%d - %d]\n\tY/R: [%d - %d]\n\n", 
                (hb - lb) * (ha - la), lb, hb, la, lb);
        }
    };
};
