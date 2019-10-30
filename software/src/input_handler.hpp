#pragma once

#include <deque>
#include <thread>
#include <mutex>

namespace altf4
{
    class InputHandler
    {
        public:
            // Public Enums

        private:
            // Private Members
            static std::deque< unsigned char >* events;
            static std::mutex* input_mutex;

        public:
            // Methods
            static void addEvent( unsigned char key );
            static bool initialize( std::deque< unsigned char >* e, std::mutex* im );

            // Accessors
            // Mutators
    };
};

