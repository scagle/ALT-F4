#pragma once
#include "data_frame.hpp"
#include "window.hpp"

#include <unordered_map>

#include <opencv2/highgui.hpp>
#include <opencv2/plot.hpp>

namespace altf4
{
    class Renderer
    {
        public:
            // Public Enums

        private:
            // Private Members
            Window window;
            std::vector< cv::Mat > mats;                           // original image per camera
            std::vector< cv::Mat > conv_mats;                      // convoultion images
            std::vector< std::vector< cv::Mat > > all_binary_mats; // multiple binary images ( one for each test ) per camera 
            // 0 = original image, 1 = HSV image, 2 = convolution image, 3+ binary image #(3 - n)
            int display_type = 0;                                       
            bool pause = false;                                       
        

        public:
            Renderer() { }
            virtual ~Renderer() { }

            // Methods
            bool initialize( int number_of_cameras );
            void renderMats( std::vector< DataFrame >* frames, std::vector< cv::Mat3b >* original_images );
            void annotateMat( int index, cv::Mat* mat, std::vector< Blob >& best_blobs );

            /* Used for debugging blob and core calculations */
            void blobAndCoreToImage( int window_index, std::vector< unsigned char* >& binary_data_2d, 
                                     Blob& blob ); 
            void writeScores( std::unordered_map< std::string, int >& scores );

            void togglePause() { this->pause ^= 1; }
            // Accessors
            std::vector< cv::Mat >* getMats() { return &mats; }
            std::vector< std::vector< cv::Mat > >* getAllBinaryMats() { return &all_binary_mats; }

            // Mutators
            void setDisplayType( unsigned char display_type );
            void toggleTune() { window.toggleTune( display_type ); }
    };
};

