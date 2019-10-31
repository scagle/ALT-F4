#include "core.hpp"

namespace altf4
{
    // Static Declarations

    // Constructors

    // Methods
    void Core::spread( std::vector< std::vector< Color > >& color_2d, std::vector< unsigned char* >& binary_data_2d )
    {
        // TODO: Really make sure you check boundaries on this one. 
        //  - Spread out in up/down + left/right + diagonals until you see valid pixel
        //  - Get radius of "core"
        //  - Get circularness of core
    }

    cv::Rect Core::getEncompassingRect( int padding ) 
    { 
        // TODO: Temp
        return cv::Rect(    origin.b - padding, 
                            origin.a - padding, 
                            ( padding * 2 ), 
                            ( padding * 2 )
                       ); 

        //return cv::Rect(    blob_min_x - padding, 
        //                    blob_min_y - padding, 
        //                    ( blob_max_x - blob_min_x ) + ( padding * 2 ), 
        //                    ( blob_max_y - blob_min_y ) + ( padding * 2 )
        //               ); 
    }
};

