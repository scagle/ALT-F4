#include "process_handler.hpp"
#include "data_frame.hpp"
#include "image.hpp"
#include "process.hpp"
#include <vector>
#include <thread>
#include <mutex>

namespace altf4
{
    // Static Declarations
    // list of all Process objects
    std::vector< Process > ProcessHandler::processes;      

    // DataFrame objects that contain results
    std::vector< std::vector< DataFrame > > ProcessHandler::frames_copies; 

    // Current index of frames_copies (alternates between 0 and 1)
    int ProcessHandler::current_copies_index;      

    // Locks frames_copies 
    std::mutex ProcessHandler::frames_copies_lock; 

    // Locks stdout
    std::mutex* ProcessHandler::print_lock;        

    void ProcessHandler::processThread( int image_index, Image& image, DataFrame& frame )
    {
        Process process( image_index, image, frame );
        process.performAlgorithms();
    }

    // Constructors

    // Methods
    void ProcessHandler::grabDataFrames( std::vector< Image >* images, 
        std::vector< DataFrame >* frames )
    {
        if ( images->size() != frames->size() )
        {
            printf("*** WARNING: Mismatch in size of images (%ld) vs frames (%ld)!  (process_handler.cpp)\n", 
                images->size(), frames->size() );
        }
        for ( unsigned int i = 0; i < images->size(); i++ )
        {
            if ( !(*images)[i].isEmpty() ) // Check if image[i] is initialized, not frames[i] 
                                           // since we want to modify it;
            {
                (*frames)[i] = DataFrame( i, &(*images)[i] ); // Reinitialize frames[i], 
                                                              // as we want to modify it

                process_threads[i] = std::thread( &ProcessHandler::processThread, i, 
                    std::ref( (*images)[i] ), std::ref( (*frames)[i] ) );
                running_threads.push_back( &process_threads[i] );
            }
        }

        for ( unsigned int i = 0; i < running_threads.size(); i++ )
        {
            running_threads[i]->join();
        }
        running_threads.clear();

    }

    void ProcessHandler::resolveThreads()
    {
        stop_threads = true;
        for ( unsigned int i = 0; i < process_threads.size(); i++ )
        {
            printf("Attempting to join Process %d\n", i);
            process_threads[i].join();
        }
    }

    bool ProcessHandler::initialize( int num_proc, std::mutex* pl )
    {
        this->number_of_processes = num_proc;
        this->print_lock = pl;
        process_threads.resize( num_proc );
        frames_copies.resize( 2 );
        for ( unsigned int i = 0; i < 2; i++ )
        {
            frames_copies[i].resize( num_proc );
        }
        return true;
    }

};

