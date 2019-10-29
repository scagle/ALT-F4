#include "data_frame.hpp"
#include "camera_handler.hpp"
#include "process_handler.hpp"
#include "keyboard_handler.hpp"
#include "serial_handler.hpp"
#include "post_process.hpp"
#include "window.hpp"
#include "data_frame.hpp"
#include "image.hpp"
#include "globals.hpp"
#include <cstdlib>
#include <cstdio>
#include <signal.h>
#include <chrono>
#include <mutex>
#include <vector>
#include <opencv2/highgui.hpp>

// Function Definitions
void grabVideoFrames( unsigned int camera_index );
void resolveAllThreads();
bool initialize();
bool initializeCameraHandler();
bool initializeProcessHandler();
bool initializePostProcess();
bool initializeWindow();
static bool initializeSignalHandler();
static void signalHandler( int signal_value );

std::mutex print_lock;  // Mutex to lock printing to console for threads

// Objects
altf4::CameraHandler camera_handler;
altf4::ProcessHandler process_handler;
altf4::PostProcess post_process;
altf4::Window window;
altf4::KeyboardHandler keyboard_handler;
altf4::SerialHandler serial_handler;

unsigned int number_of_cameras = 4;
static bool interrupted = false;
static bool done = false;

int main( int argc, char** argv )
{
    if ( !initialize() )
        return 1;

    std::vector< altf4::Image > images(number_of_cameras);
    std::vector< altf4::DataFrame > frames(number_of_cameras);
    while ( !done && !interrupted )
    {
        auto begin = std::chrono::steady_clock::now();

        // Read Images from cameras
        images = camera_handler.readImages();
        // Process Images to find lasers
        process_handler.grabDataFrames( &images, &frames ); 

        post_process.renderMats( &frames );
        window.tempDisplay( post_process.getMats() );
        //window.display( frames );

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
    if (!initializeSignalHandler()) // initialize CTRL-C and SIGTERM handling
        return false;
    if (!initializeCameraHandler())
        return false;
    if (!initializeProcessHandler())
        return false;
    if (!initializePostProcess())
        return false;
    if (!initializeWindow())
        return false;

    return true;
}

bool initializeCameraHandler()
{
    return camera_handler.initialize(number_of_cameras, &print_lock);
}

bool initializeProcessHandler()
{
    return process_handler.initialize(number_of_cameras, &print_lock);
}

bool initializePostProcess()
{
    return post_process.initialize(number_of_cameras);
}

bool initializeWindow()
{
    return window.initialize(number_of_cameras);
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

