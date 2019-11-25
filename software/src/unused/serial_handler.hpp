#pragma once
#include <string>

namespace altf4
{
    class SerialHandler
    {
        private:
            std::string path;
            int fd = -1; 

        public:
            SerialHandler() { }
            virtual ~SerialHandler() { }

            // Initialization
            bool initialize( std::string path, int baud, int parity );
            bool openUART( std::string path );
            int set_interface_attribs( int speed, int parity );

            std::string numberToString(unsigned long number);
            void writeNumber( unsigned long number );
            void writeString( std::string s );
            void set_blocking( int should_block );
    };
};

