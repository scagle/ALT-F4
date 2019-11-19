#pragma once

#include "uart.hpp"
#include "data_frame.hpp"
#include "datatypes/position.hpp"

#include <vector>
#include <thread>

namespace altf4
{
    class UARTHandler
    {
        private:
            // Private Members
            static std::vector< UART > uarts;                // List of uarts
            static std::vector< std::thread > uart_threads;  // Threads used to handle uarts
            static std::mutex data_mutex;
            static bool stop_threads;

            Position red_pos_buffer = { 320, 240 }; // Buffer used for latching onto previous laser position
            Position green_pos_buffer = { 320, 240 }; // Buffer used for latching onto previous laser position

        public:
            UARTHandler() { }
            virtual ~UARTHandler() { }

            // Methods
            static void UARTThread( int uart_index );
            bool startUART( std::string path, int baud, int parity );
            void resolveThreads();
            void prepareData( std::vector< DataFrame >& frames );
            void updateUARTData( UART::Data& data );

            // Accessors

            // Mutators
    };
};

