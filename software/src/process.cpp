#include "process.hpp"
#include "image.hpp"
#include "data_frame.hpp"

namespace altf4
{
    // Static Declarations
    static DataFrame temporary_empty_data_frame_please_fix_me_in_future;

    // Constructors
    Process::Process()
    {
        printf("*** WARNING: Created empty process with no image to process (process.cpp)\n");
    }
    Process::Process( Image* image ) : original_image(image)
    {
        processImage();
    }

    // Methods
    void Process::processImage()
    {
        image_frame = DataFrame( original_image );
    }

    DataFrame* Process::getDataFrame()
    {
        return &(image_frame);
    }
};

