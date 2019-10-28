#include "camera_handler.hpp"
#include "image.hpp"
#include <iostream>
#include <mutex>

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
    std::vector< bool > CameraHandler::updated_list;
    std::vector< bool > CameraHandler::working_list;
    
    // This is the thread used to grab camera information
    void CameraHandler::grabVideoFramesThread( unsigned int camera_index )
    {
        bool opened = cameras[camera_index].initialize( camera_index );
        while ( !stop_threads && opened )
        {
            auto begin = std::chrono::steady_clock::now();

            Image* image = cameras[camera_index].grabImage();
            image_copies_lock.lock();
            updated_list[camera_index] = true;
            image_copies[current_copies_index][camera_index] = *image; // Copy image into image_copies
            image_copies_lock.unlock();

            auto end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin );
            std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) - duration );

            print_lock->lock();
            printf( "Camera %d finished in '%ld' milliseconds\n", camera_index, duration.count() );
            print_lock->unlock();
        }
        print_lock->lock();
        printf( "Camera Thread #%d exited\n", camera_index);
        print_lock->unlock();
        working_list[camera_index] = false;
    }

    // Constructors
    
    // Methods
    std::vector< Image >* CameraHandler::readAll()
    {    
        std::vector< Image >* images;

        std::unique_lock<std::mutex> ul( image_copies_lock );
        if ( imagesReady() == false )
        {
            ul.unlock();
            while ( imagesReady() == false )
            {
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            }
            //printf("Images READY, trying to unlock!\n");
            ul.lock();
        }
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
        ul.unlock();
        for (unsigned int i = 0; i < updated_list.size(); i++)
            updated_list[i] = false;

        return images;
    }

    bool CameraHandler::imagesReady()
    {
        bool ready = true;
        for ( unsigned int i = 0; i < updated_list.size(); i++)
        {
            if (!updated_list[i] && working_list[i])  // If not updated and working
            {
                ready = false;
                break;
            }
        }
        return ready;
    }

    bool CameraHandler::initialize( unsigned int num_cam, std::mutex* pl)
    {
        print_lock = pl;
        cameras.resize( num_cam );    
        camera_threads.resize( num_cam );    
        image_copies.resize( 2 );    
        for (unsigned int i = 0; i < 2; i++)
        {
            image_copies[i].resize(num_cam);
        }
        updated_list.resize( num_cam, false );    
        working_list.resize( num_cam, true );    

        for ( unsigned int i = 0; i < num_cam; i++ )
        {
            camera_threads[i] = std::thread( &CameraHandler::grabVideoFramesThread, i );
        }
        return true;
    }
    

    void CameraHandler::resolveThreads()
    {
        stop_threads = true;
        for ( unsigned int i = 0; i < camera_threads.size(); i++ )
        {
            printf("Attempting to join Camera %d\n", i);
            camera_threads[i].join();
        }
    }

};

