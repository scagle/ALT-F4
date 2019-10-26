#include "process_handler.hpp"
#include "data_frame.hpp"
#include "image.hpp"
#include <vector>

namespace altf4
{
    // Static Declarations
    static std::vector< DataFrame> temporary_empty_frames_please_fix_me_in_future;

    // Constructors

    // Methods
    std::vector< DataFrame >* ProcessHandler::processImages( std::vector< Image >* images )
    {
        return &(temporary_empty_frames_please_fix_me_in_future);
    }
    
};

