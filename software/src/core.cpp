#include "core.hpp"

namespace altf4
{
    // Static Declarations

    // Constructors

    // Methods
    Position Core::getAnchor( std::vector< unsigned char* >& binary_data_2d, unsigned int num_rows, unsigned int num_columns, int dir_row, int dir_col)
    {
        int current_row = origin.a;
        int current_col = origin.b;
        int next_row = current_row;
        int next_col = current_col;
        int count = 0;

        do 
        { 
            next_row += dir_row;
            next_col += dir_col;
            if ( next_row <= 0 || next_row >= num_rows )
                break;
            if ( next_col <= 0 || next_col >= num_columns )
                break;
            count++;
        }
        while ( binary_data_2d[next_row][next_col] != 255 && count < 5 ); 

        next_row -= dir_row;
        next_col -= dir_col;

        return Position{ next_row, next_col };
    }

    void Core::spread( std::vector< std::vector< Color > >& color_2d, std::vector< unsigned char* >& binary_data_2d )
    {
        // TODO: Really make sure you check boundaries on this one. 
        //  - Spread out in up/down + left/right + diagonals until you see valid pixel
        //  - Get radius of "core"
        //  - Get circularness of core
           
        // Grab Anchors in clockwise manner (upperleft -> upperright -> lowerright -> lowerleft)
        anchors.push_back(getAnchor(binary_data_2d, color_2d.size(), color_2d[0].size(), -1, -1));
        anchors.push_back(getAnchor(binary_data_2d, color_2d.size(), color_2d[0].size(), -1,  0));
        anchors.push_back(getAnchor(binary_data_2d, color_2d.size(), color_2d[0].size(), -1,  1));
        anchors.push_back(getAnchor(binary_data_2d, color_2d.size(), color_2d[0].size(),  0,  1));
        anchors.push_back(getAnchor(binary_data_2d, color_2d.size(), color_2d[0].size(),  1,  1));
        anchors.push_back(getAnchor(binary_data_2d, color_2d.size(), color_2d[0].size(),  1,  0));
        anchors.push_back(getAnchor(binary_data_2d, color_2d.size(), color_2d[0].size(),  1, -1));
        anchors.push_back(getAnchor(binary_data_2d, color_2d.size(), color_2d[0].size(),  0, -1));

        for ( auto&& anchor : anchors )
        {
            printf("\t (%d, %d)\n", anchor.a, anchor.b);
        }
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

    unsigned int Core::getArea()
    {
        // Area of polygon
        // https://www.mathopenref.com/coordpolygonarea.html
        int sum = 0;
        for ( int i = 0; i < anchors.size(); i++ )
        {
            int next_i = (i + 1) % anchors.size(); // Wraps around back to (i = 0) if (i = max)
            sum += ( ( anchors[i].a * anchors[next_i].b ) - ( anchors[i].b * anchors[next_i].a ) );
        }
        return (unsigned int)(std::abs(sum) / 2);
    }
};

