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
            static std::vector< Process > processes;      // list of all Process objects
            static std::vector< std::vector< DataFrame > > frames_copies; // DataFrame objects that contain results
            static int current_copies_index;                              // Current index of frames_copies (alternates between 0 and 1)
            static std::mutex frames_copies_lock;          // Locks frames_copies 
            static std::mutex* print_lock;                 // Locks stdout

        private:
            // Private Members

            std::vector< std::thread > process_threads;    // list of threads
            unsigned int number_of_processes = 0;          // Number of process_threads
            bool stop_threads = false;
            bool image_size_mismatch_warned = false;       // A "one-time" warning that some cameras aren't working
            

        public:
            ProcessHandler() { }
            virtual ~ProcessHandler() = default;

            // Methods
            static void processImageThread( int image_index, Image& image );
            std::vector< DataFrame >* processImages( std::vector< Image >* images );
            void resolveThreads();
            bool initialize( int num_proc, std::mutex* pl );

            // Accessors

            // Mutators
    };
};

