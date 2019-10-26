#include "camera_handler.hpp"

namespace altf4
{
    // Static Declarations
    std::vector< Camera > CameraHandler::cameras;                    // List of cameras
    std::vector< std::thread > CameraHandler::camera_threads;        // Threads used to grab images from OpenCV
    std::vector< std::vector< Image > > CameraHandler::image_copies; // Copies to use for "Double-Buffer" effect
    std::mutex* CameraHandler::print_lock;                           //
    std::mutex CameraHandler::image_copies_lock;                     // To protect image_copies
    int CameraHandler::current_copies_index = 0;                     // Alternates between 0 and 1 every read
    bool CameraHandler::stop_threads = false;  
    
    // This is the thread used to grab camera information
    void grabVideoFrames( unsigned int camera_index )
    {
        CameraHandler::cameras[camera_index].initialize( camera_index );
        while ( !CameraHandler::stop_threads )
        {
            auto begin = std::chrono::steady_clock::now();

            Image* image = CameraHandler::cameras[camera_index].grabImage();
            CameraHandler::image_copies_lock.lock();
            CameraHandler::image_copies[CameraHandler::current_copies_index][camera_index] = *image; // Copy image into image_copies
            CameraHandler::image_copies_lock.unlock();

            auto end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin );
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) - duration );

            CameraHandler::print_lock->lock();
            printf( "Camera %d finished in %ld\n", camera_index, duration.count() );
            CameraHandler::print_lock->unlock();
        }
        CameraHandler::print_lock->lock();
        printf( "Camera Thread #%d exited\n", camera_index);
        CameraHandler::print_lock->unlock();
    }

    // Constructors
    CameraHandler::CameraHandler( int number_of_cameras, std::mutex* pl )
    { 
        print_lock = pl;
        cameras.resize( number_of_cameras );    
        camera_threads.resize( number_of_cameras );    
        image_copies.resize( number_of_cameras );    
    }

    // Methods
    std::vector< Image >* CameraHandler::read()
    {    
        std::vector< Image >* images;

        std::lock_guard<std::mutex> guard( image_copies_lock );
        if ( current_copies_index == 0 )
        {
            images = &(image_copies[current_copies_index]);
            current_copies_index = 1;
        }
        else
        {
            images = &(image_copies[current_copies_index]);
            current_copies_index = 0;
        }
        return images;
    }


    bool CameraHandler::initializeCameras()
    {
        for ( unsigned int i = 0; i < camera_threads.size(); i++ )
        {
            camera_threads[i] = std::thread( grabVideoFrames, i );
        }
        return true;
    }
    

    void CameraHandler::resolveThreads()
    {
        stop_threads = true;
        for ( unsigned int i = 0; i < camera_threads.size(); i++ )
        {
            printf("Attempting to close Camera %d\n", i);
            camera_threads[i].join();
        }
    }

};

