#include "data_frame.hpp"
#include "camera_handler.hpp"
#include "process_handler.hpp"
#include "keyboard_handler.hpp"
#include "serial_handler.hpp"
#include "window.hpp"
#include <cstdlib>
#include <cstdio>
#include <signal.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>

// Function Definitions
void grabVideoFrames( unsigned int camera_index );
void resolveAllThreads();
bool initialize();
bool initializeCameras();
static bool initializeSignalHandler();
static void signalHandler( int signal_value );

std::mutex print_lock;  // Mutex to lock printing to console for threads

// Objects
altf4::CameraHandler camera_handler( 1, &print_lock );
altf4::ProcessHandler process_handler;
altf4::KeyboardHandler keyboard_handler;
altf4::SerialHandler serial_handler;
altf4::Window Window;

static bool interrupted = false;
static bool done = false;

int main( int argc, char** argv )
{
    if ( !initialize() )
        return 1;

    while ( !done && !interrupted )
    {
        auto begin = std::chrono::steady_clock::now();
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin );
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) - duration );
    }
    resolveAllThreads();

    return 0;
}

void resolveAllThreads()
{
    camera_handler.resolveThreads();    
}

bool initialize()
{
    if (!initializeSignalHandler())
        return false;
    if (!initializeCameras())
        return false;

    return true;
}

bool initializeCameras()
{
    return camera_handler.initializeCameras();
}

static bool initializeSignalHandler()
{
    struct sigaction signal_handler; // For catch SIGINT or CTRL-C events
    signal_handler.sa_handler = signalHandler;
    sigemptyset( &signal_handler.sa_mask );
    signal_handler.sa_flags = 0;
    sigaction( SIGINT, &signal_handler, NULL );
    //sigaction( SIGTERM, &signal_handler, NULL );
    return true;
}

static void signalHandler( int signal_value )
{
    printf("Received Interrupt\n");
    interrupted = true;
}

