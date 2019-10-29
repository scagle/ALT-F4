#pragma once
#include "data_frame.hpp"

#include <opencv2/highgui.hpp>

namespace altf4
{
    class PostProcess
    {
        public:
        // Public Enums

        private:
        // Private Members
        std::vector< cv::Mat > mats;                       //
        std::vector< std::vector< cv::Mat > > all_binary_mats; // multiple binary images per camera ( per laser )
        

        public:
        PostProcess() { }
        virtual ~PostProcess() { }

        // Methods
        bool initialize( int number_of_cameras );
        void renderMats( std::vector< DataFrame >* frames );

        // Accessors
        std::vector< cv::Mat >* getMats() { return &mats; }
        std::vector< std::vector< cv::Mat > >* getAllBinaryMats() { return &all_binary_mats; }

        // Mutators
    };
};

