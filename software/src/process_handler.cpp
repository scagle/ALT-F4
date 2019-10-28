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
    std::vector< Process > ProcessHandler::processes;      // list of all Process objects
    std::vector< std::vector< DataFrame > > ProcessHandler::frames_copies; // DataFrame objects that contain results (
    int ProcessHandler::current_copies_index;                              // Current index of frames_copies (alternates between 0 and 1)
    std::mutex ProcessHandler::frames_copies_lock;          // Locks frames_copies 
    std::mutex* ProcessHandler::print_lock;                 // Locks stdout

    void ProcessHandler::processImageThread( int image_index, Image& image )
    {
        printf( "PHThread %d || %p, %ld \t\tcapacity: %ls\n", image_index, &frames_copies, frames_copies.size(), frames_copies.capacity() );
        Process process( &image );
        DataFrame* process_frame = process.getDataFrame();
        frames_copies_lock.lock();
        frames_copies[current_copies_index][image_index] = DataFrame(*process_frame); // Copy dataframe into frames_copies
        frames_copies_lock.unlock();
    }

    // Constructors

    // Methods
    std::vector< DataFrame >* ProcessHandler::processImages( std::vector< Image >* images )
    {
        //printf( "PHprocessImages || %p, %ld \t\tcapacity: %ls\n", &frames_copies, frames_copies.size(), frames_copies.capacity() );
        printf( "PHprocessImages || %p %ld %ld\n", &frames_copies, frames_copies.size(), frames_copies[0].size());
        printf( "%d\n", number_of_processes );
        if ( images->size() != number_of_processes && image_size_mismatch_warned == false )
        {
            printf("*** WARNING: Predicted '%d' images, but received '%ld'. Maybe some cameras aren't working? (process_handler.cpp)\n", 
                    number_of_processes, images->size());
            image_size_mismatch_warned = true;
        }

        for ( unsigned int i = 0; i < images->size(); i++ )
        {
            // Spawn thread
            process_threads[i] = std::thread( &ProcessHandler::processImageThread, i, std::ref( (*images)[i] ));
        }

        for ( unsigned int i = 0; i < images->size(); i++ )
        {
            printf("Attempting to join Process %d\n", i);
            process_threads[i].join();
        }
        std::vector< DataFrame>* frames;
        if ( current_copies_index == 0 )
        {
            frames = &(frames_copies[current_copies_index]);
            current_copies_index = 1;
        }
        else
        {
            frames = &(frames_copies[current_copies_index]);
            current_copies_index = 0;
        }


        return frames;
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
        printf( "PHConstructor || %p, %ld, %ld \t\tcapacity: %ld\n", &frames_copies, frames_copies.size(), frames_copies[0].size(), frames_copies.capacity() );
        return true;
    }
};

