#include "process.hpp"

#include "image.hpp"
#include "data_frame.hpp"
#include "algorithm.hpp"
#include "tuner.hpp"
#include <vector>

namespace altf4
{
    // Static Declarations

    // Constructors
    Process::Process( int camera_index, Image& image, DataFrame& frame ) 
        : camera_index(camera_index), image(image), frame(frame)
    {
        // Initialize DataFrame to hold appropriate number of tests (defined in tuning.hpp)
        frame.initialize( Tuner::hsv_thresholds.size() ); 
    }

    // Methods
    void Process::performAlgorithms()
    {
        // Get Binary Image
        for (unsigned int i = 0; i < Tuner::hsv_thresholds.size(); i++)
        {
            algorithm::transDimensiateImage(  // Create 2D representations of 1D arrays ( without copying )
                frame.getImage(),             // <- reference to image to be read from
                frame.getColor2D()            // <- reference to color_data_2d to be written to
            );

            algorithm::writeConvData(      // Write binary image to dataframe directly:
                frame.getColor2D(),          // <- reference to original image that binary image reads from
                frame.getConvData(),        // <- reference to binary_image to be written to
                Tuner::convolution_kernel    // <- kernel used to perform edge detection
            );

            algorithm::writeBinaryData(      // Write binary image to dataframe directly:
                frame.getImage(),            // <- reference to original image that binary image reads from
                frame.getBinaryImages()[i],  // <- reference to binary_image to be written to
                Tuner::hsv_thresholds[i]     // <- thresholds for min/max boundaries of binary_image
            );

            algorithm::transDimensiateBinaryImage(  // Create 2D representations of 1D arrays ( without copying )
                frame.getBinaryImages()[i],         // <- reference to binary_image to be read from
                frame.getBinaryDatas2D()[i]         // <- reference to binary_data_2d to be written to
            );

            algorithm::getBlobs(             // Grab blobs from binary_images
                frame.getImage(),            // <- reference to original image that we get information from (#rows, #cols, pixeldata)
                frame.getColor2D(),          // <- reference to color_2d to be read from
                frame.getBinaryDatas2D()[i], // <- reference to binary_data_2d to be read from
                frame.getAllBlobs()[i]       // <- reference to all_blobs to written to
            );

            algorithm::scoreBlobs(              // Score blobs based on attributes
                frame.getColor2D(),             // <- reference to color_data_2d to be written to
                frame.getBinaryDatas2D()[i],    // <- reference to binary_data_2d to be written to
                frame.getAllBlobs()[i],         // <- reference to blobs the read from
                frame.getBestBlobs()[i],        // <- reference to best blob to write to
                i                               // <- index of type
            );
        }
    }
};

