#pragma once
#include "data_frame.hpp"
#include "image.hpp"
#include <vector>

namespace altf4
{
    class ProcessHandler
    {
        public:
            // Public Enums

        private:
            // Private Members
            

        public:
            ProcessHandler() { }
            virtual ~ProcessHandler() { }

            // Methods
            std::vector< DataFrame >* processImages( std::vector< Image >* images );

            // Accessors

            // Mutators
    };
};

