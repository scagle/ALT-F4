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
            int number_of_images = 0;
            

        public:
            Window() { }
            Window( int num ) : number_of_images(num) { }
            virtual ~Window() { }

            // Methods
            bool initializeWindow();
            void temp( std::vector< Image >* images );
            void display( std::vector< DataFrame >* );

            // Accessors

            // Mutators
    };
};

