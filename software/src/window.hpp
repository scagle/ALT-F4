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
            int tune_type = 0;
            bool handle_tune = false;
            bool update_tune = false;
            int test = 0;

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
            void updateTune( int display_type );

            // Accessors

            // Mutators
    };
};

