#include "core.hpp"

#include "datatypes/color.hpp"
#include "datatypes/core_anchor.hpp"

namespace altf4
{
    // Static Declarations

    // Constructors

    // Methods
    CoreAnchor Core::getAnchor( std::vector< unsigned char* >& binary_data_2d, std::vector< std::vector< Color > >& color_2d, 
            unsigned int num_rows, unsigned int num_columns, int dir_row, int dir_col)
    {
        int current_row = origin.a;
        int current_col = origin.b;
        int next_row = current_row;
        int next_col = current_col;
        int length = 0;
        int channel_a_sum = 0;
        int channel_b_sum = 0;
        int channel_c_sum = 0;
        bool failed = false;

        do 
        { 
            next_row += dir_row;
            next_col += dir_col;

            // Check if core "exploded" (exceeded blob-size)
            if ( next_row <= blob_boundary->la || next_row >= blob_boundary->ha )
            {
                failed = true;
                break;
            }
            if ( next_col <= blob_boundary->lb || next_col >= blob_boundary->hb )
            {
                failed = true;
                break;
            }
            channel_a_sum += color_2d[next_row][next_col].b;
            channel_b_sum += color_2d[next_row][next_col].g;
            channel_c_sum += color_2d[next_row][next_col].r;

            length++;
        }
        while ( binary_data_2d[next_row][next_col] != 255 ); 

        // Backtrack a step since we ended on invalid pixel
        next_row -= dir_row;
        next_col -= dir_col;

        // Set core boundaries
        if ( core_boundary.la > next_row ) // If row is lower than Core's lowest row
            core_boundary.la = next_row;
        if ( core_boundary.lb > next_col ) // If col is lower than Core's lowest col
            core_boundary.lb = next_col;
        if ( core_boundary.ha < next_row ) // If row is higher than Core's highest row
            core_boundary.ha = next_row;
        if ( core_boundary.hb < next_col ) // If col is higher than Core's highest col
            core_boundary.hb = next_col;

        // Get average color
        Color avg_color = {-1, -1, -1};
        if (length > 0)
        {
            channel_a_sum /= length;
            channel_b_sum /= length;
            channel_c_sum /= length;
            avg_color = { channel_a_sum, channel_b_sum, channel_c_sum };
        }

        // Check if exploded
        if ( failed )
            this->exploded = true;
        
        return CoreAnchor{ { next_row, next_col }, avg_color, length, failed };
    }

    void Core::spread( std::vector< std::vector< Color > >& color_2d, std::vector< unsigned char* >& binary_data_2d )
    {
        // TODO: Really make sure you check boundaries on this one. 
        //  - Spread out in up/down + left/right + diagonals until you see valid pixel
        //  - Get radius of "core"
        //  - Get circularness of core

        // Start core boundary at origin
        core_boundary = { origin.a, origin.b, origin.a, origin.b };
           
        // Grab Anchors in clockwise manner (upperleft -> upperright -> lowerright -> lowerleft)
        anchors.push_back( getAnchor( binary_data_2d, color_2d, color_2d.size(), color_2d[0].size(), -1, -1 ) );
        anchors.push_back( getAnchor( binary_data_2d, color_2d, color_2d.size(), color_2d[0].size(), -1,  0 ) );
        anchors.push_back( getAnchor( binary_data_2d, color_2d, color_2d.size(), color_2d[0].size(), -1,  1 ) );
        anchors.push_back( getAnchor( binary_data_2d, color_2d, color_2d.size(), color_2d[0].size(),  0,  1 ) );
        anchors.push_back( getAnchor( binary_data_2d, color_2d, color_2d.size(), color_2d[0].size(),  1,  1 ) );
        anchors.push_back( getAnchor( binary_data_2d, color_2d, color_2d.size(), color_2d[0].size(),  1,  0 ) );
        anchors.push_back( getAnchor( binary_data_2d, color_2d, color_2d.size(), color_2d[0].size(),  1, -1 ) );
        anchors.push_back( getAnchor( binary_data_2d, color_2d, color_2d.size(), color_2d[0].size(),  0, -1 ) );

        // Update global avg_color of anchors
        int channel_a_sum = 0;
        int channel_b_sum = 0;
        int channel_c_sum = 0;
        int length_sum = 0;
        int valid_count = 0;

        for ( auto&& anchor : anchors )
        {
            if ( anchor.avg_color.b == -1 )
                continue;
            channel_a_sum += anchor.avg_color.b;
            channel_b_sum += anchor.avg_color.g;
            channel_c_sum += anchor.avg_color.r;
            length_sum += anchor.length;
            valid_count++;
        }

        if ( valid_count == 0 ) // Avoiding arithmetic exception
        {
            this->avg_color = color_2d[origin.a][origin.b];
            this->avg_length = 0;
        }
        else
        {
            channel_a_sum /= valid_count;
            channel_b_sum /= valid_count;
            channel_c_sum /= valid_count;
            length_sum /= valid_count;
            this->avg_color = { channel_a_sum, channel_b_sum, channel_c_sum };
            this->avg_length = length_sum; 
        }

        //for ( auto&& anchor : anchors )
        //{
        //    printf("\t (%d, %d)\n", anchor.a, anchor.b);
        //}
    }

    cv::Rect Core::getEncompassingRect( int padding ) 
    { 
        // Note: 
        // For future viewers, OpenCV uses X / Y notation instead of R / C for drawing rectangles.
        // To emphasize this, below is how R / C is translated into X / Y:
        const int core_min_x = core_boundary.lb;
        const int core_min_y = core_boundary.la;
        const int core_max_x = core_boundary.hb;
        const int core_max_y = core_boundary.ha;

        return cv::Rect(    core_min_x - padding, 
                            core_min_y - padding, 
                            ( core_max_x - core_min_x ) + ( padding * 2 ), 
                            ( core_max_y - core_min_y ) + ( padding * 2 )
                       ); 
    }

    unsigned int Core::getArea()
    {
        // Area of polygon
        // https://www.mathopenref.com/coordpolygonarea.html
        int sum = 0;
        for ( int i = 0; i < anchors.size(); i++ )
        {
            int next_i = (i + 1) % anchors.size(); // Wraps around back to (i = 0) if (i = max)
            sum += ( ( anchors[i].anchor.a * anchors[next_i].anchor.b ) - ( anchors[i].anchor.b * anchors[next_i].anchor.a ) );
        }
        return (unsigned int)(std::abs(sum) / 2);
    }
};

