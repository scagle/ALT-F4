#include "camera_handler.hpp"
#include "image.hpp"
#include <iostream>
#include <mutex>
#include <deque>

namespace altf4
{
    // Static Declarations
    std::vector< Camera > CameraHandler::cameras;                    // List of cameras
    std::vector< std::thread > CameraHandler::camera_threads;        // Threads used to grab images from OpenCV
    std::vector< Image > CameraHandler::images;                      //  
    std::mutex* CameraHandler::print_lock;                           //
    std::mutex CameraHandler::image_lock;                            // To protect images
    //std::mutex CameraHandler::fps_lock;                            // To protect FPS 
    bool CameraHandler::stop_threads = false;  
    std::vector< bool > CameraHandler::updated_list;
    std::vector< bool > CameraHandler::working_list;
    //std::vector< std::deque< int > > CameraHandler::fps_timing;
    
    // This is the thread used to grab camera information
    void CameraHandler::grabVideoFramesThread( unsigned int camera_index )
    {
        bool opened = cameras[camera_index].initialize( camera_index );
        while ( !stop_threads && opened )
        {
            auto begin = std::chrono::steady_clock::now();

            Image* image = cameras[camera_index].grabImage();
            image_lock.lock();
            images[camera_index] = *image; // Copy image into images
            updated_list[camera_index] = true;
            image_lock.unlock();

            auto end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin );
            std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) - duration );

            //fps_lock.lock();
            //fps_timing[camera_index].push_front(duration.count());  // Keep track of time
            //if ( fps_timing[camera_index].size() > 1000 )           // Don't keep track of too much time
            //    fps_timing[camera_index].pop_back();
            //fps_lock.unlock();
        }
        print_lock->lock();
        printf( "Camera Thread #%d exited\n", camera_index);
        print_lock->unlock();
        working_list[camera_index] = false;
    }

    // Constructors
    
    // Methods
    std::vector< Image > CameraHandler::readImages()
    {    
        std::unique_lock<std::mutex> ul( image_lock );
        // Check if all cameras have had a chance to write their capture to images
        if ( imagesReady() == false )
        {
            // Wait until all cameras have had a chance to write
            ul.unlock();
            while ( imagesReady() == false )
            {
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
            }
            //printf("Images READY, trying to unlock!\n");
            ul.lock();
        }

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
        images.resize( num_cam );    
        updated_list.resize( num_cam, false );    
        working_list.resize( num_cam, true );    

        //fps_timing.resize( num_cam );    

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
            //unsigned long sum = 0;
            //for ( unsigned int j = 0; j < fps_timing[i].size(); j++ )
            //{
            //    sum += fps_timing[i][j];
            //}
            // 
            //if ( fps_timing[i].size() > 0 )
            //    printf("Camera '%d' ended with an average fps of around '%ld'\n", i, sum / fps_timing[i].size());
            //else
            //    printf("Camera '%d' never ran\n", i);
        }
    }

};

