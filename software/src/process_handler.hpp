#pragma once
#include "data_frame.hpp"
#include "image.hpp"
#include "process.hpp"
#include <vector>
#include <thread>
#include <mutex>

namespace altf4
{
    class ProcessHandler
    {
        public:
            // list of all Process objects
            static std::vector< Process > processes;      

            // DataFrame objects that contain results
            static std::vector< std::vector< DataFrame > > frames_copies; 

            // Current index of frames_copies (alternates between 0 and 1)
            static int current_copies_index;                              

            // Locks frames_copies 
            static std::mutex frames_copies_lock;          

            // Locks stdout
            static std::mutex* print_lock;                 

        private:
            // Private Members

            std::vector< std::thread > process_threads;    // list of threads
            std::vector< std::thread* > running_threads;   // list of running threads
            unsigned int number_of_processes = 0;          // Number of process_threads
            bool stop_threads = false;
            bool image_size_mismatch_warned = false;       // A "one-time" warning that some 
                                                           // cameras aren't working
                                                           
        public:
            ProcessHandler() { }
            virtual ~ProcessHandler() { }

            // Methods
            static void processThread( int image_index, Image& image, DataFrame& frame );
            void grabDataFrames( std::vector< Image >* images, std::vector< DataFrame >* frames );
            void resolveThreads();
            bool initialize( int num_proc, std::mutex* pl );
            static bool hasNewData();
            static std::vector< Position > getCoordinates();

            // Accessors

            // Mutators
    };
};

