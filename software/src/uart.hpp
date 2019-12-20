#pragma once

#include "datatypes/position.hpp"

#include <string>
#include <vector>
#include <mutex>
#include <termios.h>

namespace altf4
{
    class UART
    {
        public:
            enum class State
            {
                GREEN_LASER,
                RED_LASER,
                BOTH_LASER,
                NO_LASER,
            };

            struct Data
            {
                std::vector< Position > coords;
                State state;
            };
            bool debug = false; // flag used aid in debugging 

        private:
            // Private Members
            std::string path = "";
            int fd = -1;
            speed_t baud = B115200;
            int parity = 0;
            bool opened = false;
            bool new_data_flag = false;

            // Critical Section
            std::mutex* data_mutex;
            Data data; // Critical section component that stores data to be sent

            // Private Methods
            bool openUART();
            int set_interface_attribs (speed_t speed, int parity);
            void set_blocking( int should_block );

        public:
            UART( std::string path, speed_t baud, int parity, std::mutex* dm ) : 
                path(path), baud(baud), parity(parity), data_mutex(dm) { }
            virtual ~UART() { }

            // Methods
            std::string headerNumberToString( std::string header, unsigned long number );
            std::string numberToString( unsigned long number );
            void writeHeaderNumber( std::string header, unsigned long number );
            void writeNumber( unsigned long number );
            void writeString( std::string str );

            bool initialize();
            bool hasNewData(); 
            Data getData();             // Return a copy thread-safely
            void setData( Data& data ); // set data thread-safely
            void sendData();            // Send newly-received data over UART

            // Accessors
            std::string getPath() { return this->path; }
            int getFileDescriptor() { return this->fd; }
            int getBaud() { return this->baud; }
            int getParity() { return this->parity; }
            int isOpened() { return this->opened; }

            // Mutators
    };
};

