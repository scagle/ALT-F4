#include "uart_handler.hpp"

#include "uart.hpp"
#include "uart_logic.hpp"
#include "blob.hpp"

#include "datatypes/position.hpp"
namespace altf4
{
    // Static Declarations
    std::vector< UART > UARTHandler::uarts;                // List of uarts
    std::vector< std::thread > UARTHandler::uart_threads;  // Threads used to handle uarts
    std::mutex UARTHandler::data_mutex;
    bool UARTHandler::stop_threads = false;

    // Constructors

    // Methods
    void UARTHandler::UARTThread( UART& uart )
    {
        bool opened = uart.initialize();
        if (!opened)
        {
            printf("Could not open UART to '%s'!\n", uart.getPath().c_str());
        }
        while ( !stop_threads && opened )
        {
            if ( uart.hasNewData() )
            {
                uart.sendData();
            }
        }
        printf("UART Thread '%s' exited\n", uart.getPath().c_str());
    }

    bool UARTHandler::startUART( std::string path, int baud, int parity )
    {
        uarts.push_back(UART( path, baud, parity, &data_mutex ));
        uart_threads.push_back( std::thread( &UARTHandler::UARTThread, std::ref( uarts[uarts.size()-1] ) ) );
        printf("Started UART '%s' Thread!\n", path.c_str());
        return true;
    }

    void UARTHandler::resolveThreads()
    {
        stop_threads = true;
        //TODO: Errors here, and can't write to uart on top of that
        printf("Attempting to join UART \n");
        for ( unsigned int i = 0; i < uart_threads.size(); i++ )
        {
            printf("Attempting to join UART %d Thread out of %ls\n", i, uart_threads.size());
            uart_threads[i].join();
        }
    }

    // Prioritize cameras based on rules
    // Front Camera: Highest Priority
    // Side Cameras: Medium Priority
    // Back Camera:  Lowest Priority
    // TODO: Hardcoding it for now
    void UARTHandler::prepareData( std::vector< DataFrame >& frames )
    {
        UART::Data best_data = { { {0, 0}, {0, 0} }, UART::State::NO_LASER };

        for ( auto&& frame : frames )
        {
            if ( frame.isInitialized() == false )
                continue;
            std::vector< Blob >& best_blobs = frame.getBestBlobs();
            Blob& green_blob = best_blobs[0];
            Blob& red_blob = best_blobs[1];
            bool green_found = green_blob.isInitialized();
            bool red_found = red_blob.isInitialized();

            // Determine State
            UART::State state;
            if ( green_found && red_found )
            {
                state = UART::State::BOTH_LASER;
            }
            else if ( green_found )
            {
                state = UART::State::GREEN_LASER;
            }
            else if ( red_found )
            {
                state = UART::State::RED_LASER;
            }
            else
            {
                state = UART::State::NO_LASER;
            }

            // Update Buffers if necessary
            if ( green_found )
            {
                green_pos_buffer = green_blob.getCenterPosition();
            }
            if ( red_found )
            {
                red_pos_buffer = red_blob.getCenterPosition();
            }

            // Formulate Data
            if ( state != UART::State::NO_LASER && state != UART::State::RED_LASER)
            {
                best_data = { { green_pos_buffer, red_pos_buffer }, state };
                break; // Since highest priority starts at beginning, we now no longer have to continue with for-loop iterations
            }
        }
        updateUARTData( best_data );
    }

    void UARTHandler::updateUARTData( UART::Data& data )
    {
        for ( auto&& uart : uarts )
        {
            uart.setData( data );
        }
    }
};

