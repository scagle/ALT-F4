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
        frame.initialize( camera_index, Tuner::hsv_thresholds[0].size() ); 
    }

    // Methods
    void Process::performAlgorithms()
    {
        // Get Binary Image
        for (unsigned int i = 0; i < Tuner::hsv_thresholds[0].size(); i++)
        {
            // Create 2D representations of 1D arrays ( without copying )
            algorithm::transDimensiateImage(  
                frame.getImage(),             // <- reference to image to be read from
                frame.getColor2D()            // <- reference to color_data_2d to be written to
            );


            // Write binary image to dataframe directly:
            algorithm::writeBinaryData(      
                frame.getImage(),            // <- reference to original image read from
                frame.getBinaryImages()[i],  // <- reference to binary_image to be written to
                Tuner::hsv_thresholds[camera_index][i] // <- thresholds for min/max boundaries 
                                                       //    of binary_image
            );

            // Create 2D representations of 1D arrays ( without copying )
            algorithm::transDimensiateBinaryImage(  
                frame.getBinaryImages()[i],         // <- reference to binary_image read from
                frame.getBinaryDatas2D()[i]         // <- reference to binary_data_2d written to
            );

            // Grab blobs from binary_images
            algorithm::getBlobs(             
                frame.getImage(),            // <- reference to original image that we get 
                                             //    information from (#rows, #cols, pixeldata)
                frame.getColor2D(),          // <- reference to color_2d to be read from
                frame.getBinaryDatas2D()[i], // <- reference to binary_data_2d to be read from
                frame.getAllBlobs()[i]       // <- reference to all_blobs to written to
            );

            // Score blobs based on attributes
            algorithm::scoreBlobs(              
                frame.getColor2D(),             // <- reference to color_data_2d to be written to
                frame.getBinaryDatas2D()[i],    // <- reference to binary_data_2d to be written to
                frame.getAllBlobs()[i],         // <- reference to blobs the read from
                frame.getBestBlobs()[i],        // <- reference to best blob to write to
                i,                              // <- index of type
                camera_index                    // <- camera index
            );
        }
    }
};

