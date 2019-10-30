#pragma once
#include "data_frame.hpp"
#include "window.hpp"

#include <opencv2/highgui.hpp>

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
        std::vector< std::vector< cv::Mat > > all_binary_mats; // multiple binary images ( one for each test ) per camera 
        int type = 0;                                       // 0 = image, 1 = binary image
        

        public:
        Renderer() { }
        virtual ~Renderer() { }

        // Methods
        bool initialize( int number_of_cameras );
        void renderMats( std::vector< DataFrame >* frames, std::vector< cv::Mat3b >* original_images );
        void annotateMat( int index, cv::Mat* mat );

        // Accessors
        std::vector< cv::Mat >* getMats() { return &mats; }
        std::vector< std::vector< cv::Mat > >* getAllBinaryMats() { return &all_binary_mats; }

        // Mutators
        void setType( unsigned char type ) { this->type = type; }
    };
};

