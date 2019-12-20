#pragma once

// Debugging 
#define DEBUG              1        // Enables certain print statements

// Camera Options
#define MIN_ROW            0 
#define MIN_COL            0
#define MAX_ROW          480
#define MAX_COL          640
#define FPS               30
#define BUFFERSIZE         1

// Blob Detection
#define MAX_RELATIVE_OFFSET    1    // How big the radius is for checking neighbor pixels 
                                    // ( 1 = check 8 neighbors, 2 = check 16 neighbors, ... )
#define EDGE_NEIGHBOR_CUTOFF   8    // If pixel has less neighbors than this value, then it is 
                                    // an edge pixel ( otherwise it is core pixel )
