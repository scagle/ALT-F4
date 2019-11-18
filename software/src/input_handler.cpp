#include "input_handler.hpp"

namespace altf4
{
    // Static Declarations
    std::deque< unsigned char >* InputHandler::events;
    std::mutex* InputHandler::input_mutex;

    // Constructors

    // Methods
    void InputHandler::addEvent( unsigned char key )
    {
        std::lock_guard< std::mutex > lock( *input_mutex );
        events->push_front( key );
    }

    bool InputHandler::initialize( std::deque< unsigned char >* e, std::mutex* im )
    {
        events = e;
        input_mutex = im;
        return true;
    }
};

