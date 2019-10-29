#include "process.hpp"

#include "image.hpp"
#include "data_frame.hpp"
#include "algorithm.hpp"
#include "tuning.hpp"
#include <vector>

namespace altf4
{
    // Static Declarations

    // Constructors
    Process::Process( int camera_index, Image& image, DataFrame& frame ) 
        : camera_index(camera_index), image(image), frame(frame)
    {
        // Initialize DataFrame to hold appropriate number of tests (defined in tuning.hpp)
        frame.initialize( tuning::hsv_thresholds.size() ); 
    }

    // Methods
    void Process::performAlgorithms()
    {
        // Get Binary Image
        for (unsigned int i = 0; i < tuning::hsv_thresholds.size(); i++)
        {
            algorithm::writeBinaryData(      // Write binary image to dataframe directly:
                frame.getImage(),            // <- reference to original image that binary image reads from
                frame.getBinaryImages()[i],  // <- reference to binary_image to be written to
                tuning::hsv_thresholds[i]    // <- thresholds for min/max boundaries of binary_image
            );

            algorithm::transDimensiateImage(  // Create 2D representations of 1D arrays ( without copying )
                frame.getImage(),             // <- reference to binary_image to be read from
                frame.getColor2D()            // <- reference to binary_data_2d to be written to
            );

            algorithm::transDimensiateBinaryImage(  // Create 2D representations of 1D arrays ( without copying )
                frame.getBinaryImages()[i],         // <- reference to binary_image to be read from
                frame.getBinaryDatas2D()[i]         // <- reference to binary_data_2d to be written to
            );

            algorithm::getBlobs(             // Create 2D representations of 1D arrays ( without copying )
                frame.getImage(),            // <- reference to original image that we get information from (#rows, #cols, pixeldata)
                frame.getColor2D(),          // <- reference to color_2d to be read from
                frame.getBinaryDatas2D()[i], // <- reference to binary_data_2d to be read from
                frame.getAllBlobs()[i]       // <- reference to all_blobs to written to
            );
        }
    }
};

