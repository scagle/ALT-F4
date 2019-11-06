#pragma once
#include "data_frame.hpp"
#include <vector>


namespace altf4
{
    class Window
    {
        public:
            // Public Enums

        private:
            // Private Members
            std::vector< std::string > names;
            std::vector< cv::Mat* > matrices;
            unsigned int number_of_images = 0;
            bool tune = false;
            

        public:
            Window() { }
            Window( int num ) : number_of_images(num) { }
            virtual ~Window() { }

            // Methods
            bool initialize(unsigned int);
            void temp( std::vector< Image >* images );
            void tempDisplay( std::vector< cv::Mat >* mats );
            void render( unsigned int index, cv::Mat* mat );
            void toggleTune( int type );    

            // Accessors

            // Mutators
    };
};

