#include "uart.hpp"

#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <mutex>

namespace altf4
{
    // Static Declarations

    // Constructors

    // Private Methods 
    bool UART::openUART()
    {
        this->fd = open( path.c_str(), O_RDWR | O_NOCTTY | O_SYNC );

        if ( this->path == "" )
        {
            printf("BIG ANNOYING DEBUG MESSAGE THAT I WILL HOPEFULLY SEE IN THE TERMINAL WHEN THIS HAPPENS!\n");
        }
        if ( fd < 0 )
        {
            opened = false;
            printf( "Error opening '%s'\n", path.c_str());
            return false;
        }
        opened = true;
        return true;
    }

    int UART::set_interface_attribs( int speed, int parity )
    {
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
            printf("Error from tcgetattr\n");
            return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
            printf("Error from tcsetattr\n");
            return -1;
        }
        return 0;
    }

    void UART::set_blocking( int should_block )
    {
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
            printf("Error from tggetattr\n");
            return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
            printf("Error setting term attributes\n");
    }

    std::string UART::numberToString( unsigned long number )
    {
        std::ostringstream oss;
        oss << number << "\r";
        return oss.str();
    }
    
    void UART::writeNumber( unsigned long number )
    {
        const char *num_string = numberToString(number).c_str();
        write( fd, num_string, strlen(num_string));
    }

    void UART::writeString( std::string str )
    {
        const char *char_string = str.c_str();
        write( fd, char_string, strlen(char_string) );
    }

    // Public Methods
    bool UART::initialize()
    {
        if ( openUART() == false )
        {
            return false;
        }
        else
        {
            set_interface_attribs( this->baud, this->parity );   // set speed to 115,200 bps, 8n1 (no parity)
            set_blocking( 0 );                                   // set no blocking
        }
        return true;
    }

    bool UART::hasNewData()
    {
        return this->new_data_flag;
    }

    UART::Data UART::getData()
    {
        std::lock_guard< std::mutex > guard( *data_mutex ); // Enter Critical Section (exception safe)
        return this->data; // return copy of data, and leave Critical Section
    }

    void UART::setData( UART::Data& data )
    {
        std::lock_guard< std::mutex > guard( *data_mutex ); // Enter Critical Section (exception safe)
        if ( debug && new_data_flag )
        {
            printf("*** WARNING: Data Overwritten! (uart.cpp)\n");
        }
        this->new_data_flag = true;
        this->data = data;
    }

    void UART::sendData()
    {
        switch ( data.state )
        {
            case UART::State::GREEN_LASER : 
            case UART::State::BOTH_LASER : 
            {
                writeString("strt\r");
                writeNumber( data.coords[0].a );
                writeNumber( data.coords[0].b );
                writeNumber( data.coords[1].a );
                writeNumber( data.coords[1].b );
                writeString("stop\r");
                break;
            }
            case UART::State::NO_LASER : 
            case UART::State::RED_LASER : 
            {
                writeString("strt\r");
                writeString("none\r");
                writeString("stop\r");
                break;
            }
            default:
            {
                printf( "*** WARNING: Undefined Behavior (uart.cpp)\n" );
            }
        }
        new_data_flag = false;
        printf( "Wrote data on UART\n" );
    }
};

