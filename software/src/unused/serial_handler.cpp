#include "serial_handler.hpp"

#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sstream>


namespace altf4
{
    bool SerialHandler::initialize( std::string path, int baud, int parity )
    {
        if ( openUART( path ) == false )
        {
            printf( "Error opening '%s'\n", path.c_str() );
            return false;
        }
        else
        {
            set_interface_attribs( baud, parity );   // set speed to 115,200 bps, 8n1 (no parity)
            set_blocking( 0 );                       // set no blocking
        }
        return true;
    }

    bool SerialHandler::openUART( std::string path )
    {
        this->path = path;
        this->fd = open( path.c_str(), O_RDWR | O_NOCTTY | O_SYNC );

        if ( fd < 0 )
        {
            printf( "Error opening '%s'\n", path.c_str());
            return false;
        }
        return true;
    }

    int SerialHandler::set_interface_attribs (int speed, int parity)
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

    std::string SerialHandler::numberToString(unsigned long number)
    {
        std::ostringstream oss;
        oss << number << "\r\n";
        return oss.str();
    }

    void SerialHandler::writeNumber( unsigned long number )
    {
        const char *num_string = numberToString(number).c_str();
        write( fd, num_string, strlen(num_string));
    }

    void SerialHandler::writeString( std::string str )
    {
        std::ostringstream oss;
        oss << str << "\r\n";
        write( fd, oss.str().c_str(), strlen( oss.str().c_str() ) );
    }

    void SerialHandler::set_blocking (int should_block)
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
};

