#include "data_frame.hpp"
#include "camera_handler.hpp"
#include "process_handler.hpp"
#include "input_handler.hpp"
#include "serial_handler.hpp"
#include "renderer.hpp"
#include "data_frame.hpp"
#include "image.hpp"
#include "globals.hpp"
#include <cstdlib>
#include <cstdio>
#include <signal.h>
#include <chrono>
#include <mutex>
#include <vector>
#include <deque>
#include <opencv2/highgui.hpp>

// Function Definitions
void grabVideoFrames( unsigned int camera_index );
void resolveAllThreads();
bool initialize();
bool initializeCameraHandler();
bool initializeProcessHandler();
bool initializeRenderer();
bool initializeInput();
void handleInputsThread( std::deque< unsigned char >& events, bool& done, bool& interrupted );
static bool initializeSignalHandler();
static void signalHandler( int signal_value );

std::mutex print_lock;  // Mutex to lock printing to console for threads

// Objects
altf4::CameraHandler camera_handler;
altf4::ProcessHandler process_handler;
altf4::Renderer renderer;
altf4::SerialHandler serial_handler;

// Input Handling
std::thread input_thread;
std::mutex input_mutex;
std::deque< unsigned char > events;

unsigned int number_of_cameras = 4;
static bool interrupted = false;
static bool done = false;
static bool tune = false;    // Toggles tune menu

int main( int argc, char** argv )
{
    if ( !initialize() )
        return 1;

    std::vector< cv::Mat3b > original_images(number_of_cameras);
    std::vector< altf4::Image > images(number_of_cameras);
    std::vector< altf4::DataFrame > frames(number_of_cameras);

    while ( !done && !interrupted )
    {
        auto begin = std::chrono::steady_clock::now();

        // Read Images from cameras
        camera_handler.readImages( &original_images, &images );
        // Process Images to find lasers
        process_handler.grabDataFrames( &images, &frames ); 
        renderer.renderMats( &frames, &original_images );

        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin );
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) - duration );
    }
    resolveAllThreads();

    return 0;
}

void handleInputsThread( std::deque< unsigned char >& events, bool& done, bool& interrupted )
{
    while ( !done && !interrupted )
    {
        // events written directly to by InputHandler
        if ( !events.empty() ) 
        {
            switch ( events.back() )
            {
                case 'q':
                {
                    done = true;
                    break;
                }
                case 't':
                {
                    tune = !tune;
                    break;
                }

                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                {
                    renderer.setDisplayType( events.back() - 48 );  // 48 = ascii value for zero + offset
                    break;
                }
            }
            std::lock_guard< std::mutex > lock( input_mutex );
            events.pop_back();
        }
    }
    printf("Input Thread exited\n");
}

void resolveAllThreads()
{
    camera_handler.resolveThreads();    
    printf("Attempting to join Input Thread\n");
    input_thread.join();
}

bool initialize()
{
    if (!initializeSignalHandler()) // initialize CTRL-C and SIGTERM handling
        return false;
    if (!initializeCameraHandler())
        return false;
    if (!initializeProcessHandler())
        return false;
    if (!initializeRenderer())
        return false;
    if (!initializeInput())
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

bool initializeRenderer()
{
    return renderer.initialize(number_of_cameras);
}

bool initializeInput()
{
    input_thread = std::thread( handleInputsThread, std::ref( events ), std::ref( done ), std::ref( interrupted ) );
    return altf4::InputHandler::initialize( &events, &input_mutex );
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

