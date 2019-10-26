#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sstream>
#include "serial.hpp"

std::string SerialHandler::numberToString(unsigned long number)
{
    std::ostringstream oss;
    oss << number << "\r";
    return oss.str();
}

void SerialHandler::writeNumber( unsigned long number )
{
    const char *num_string = numberToString(number).c_str();
    write( fd, num_string, strlen(num_string));
}

void SerialHandler::writeString( std::string str )
{
    const char *char_string = str.c_str();
    write( fd, char_string, strlen(char_string) );
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

//#include <errno.h>
//#include <fcntl.h> 
//#include <string.h>
//#include <termios.h>
//#include <unistd.h>
//#include <stdlib.h>
//#include <stdint.h>
//#include <stdio.h>
//#include <string>
//#include "serial.hpp"
//
//int SerialHandler::set_interface_attribs(int fd, int speed, int parity)
//{
//    struct termios tty;
//    memset (&tty, 0, sizeof tty);
//    if (tcgetattr (fd, &tty) != 0)
//    {
//        fprintf (stderr, "error %d from tcgetattr\n", errno);
//        return -1;
//    }
//
//    cfsetospeed (&tty, speed);
//    cfsetispeed (&tty, speed);
//
//    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
//    // disable IGNBRK for mismatched speed tests; otherwise receive break
//    // as \000 chars
//    tty.c_iflag &= ~IGNBRK;         // disable break processing
//    tty.c_lflag = 0;                // no signaling chars, no echo,
//    // no canonical processing
//    tty.c_oflag = 0;                // no remapping, no delays
//    tty.c_cc[VMIN]  = 0;            // read doesn't block
//    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
//
//    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
//
//    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
//    // enable reading
//    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
//    tty.c_cflag |= parity;
//    tty.c_cflag &= ~CSTOPB;
//    tty.c_cflag &= ~CRTSCTS;
//
//    if (tcsetattr (fd, TCSANOW, &tty) != 0)
//    {
//        fprintf (stderr, "error %d from tcsetattr\n", errno);
//        return -1;
//    }
//    return 0;
//}
//
//void SerialHandler::set_blocking(int fd, int should_block)
//{
//    struct termios tty;
//    memset (&tty, 0, sizeof tty);
//    if (tcgetattr (fd, &tty) != 0)
//    {
//        fprintf (stderr, "error %d from tggetattr\n", errno);
//        return;
//    }
//
//    tty.c_cc[VMIN]  = should_block ? 1 : 0;
//    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
//
//    if (tcsetattr (fd, TCSANOW, &tty) != 0)
//        fprintf (stderr, "error %d setting term attributes", errno);
//}
//
//void SerialHandler::uart_writestr(const char* string) {
//    write(uartFd, string, sizeof(string));
//}	
//
//void SerialHandler::uart_write(void* data, size_t len) {
//    write(uartFd, data, len); 
//}
//
//ssize_t SerialHandler::uart_read(void* buffer, size_t charsToRead) {
//    return read(uartFd, buffer, charsToRead); 
//}
//
//int SerialHandler::uart_open(const char* port, int baud, int blocking) {
//    uartFd = open(port, O_RDWR | O_NOCTTY | O_SYNC);
//    if (uartFd < 0)
//    {
//        fprintf (stderr, "error %d opening %s: %s\n", errno, port, strerror (errno));
//        return -1;
//    }
//    set_interface_attribs (uartFd, baud, 0);  // set speed, 8n1 (no parity)
//    set_blocking (uartFd, blocking); //set blocking mode
//    printf("Port %s opened.\n", port); 
//    return 1;
//}
//
//void SerialHandler::uart_close() 
//{
//    close( uartFd );
//}
//
////void SerialHandler::openSerial( std::string device )
////{
////    fd = open( device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY );
////    if ( fd == -1 )
////    {
////        printf( "\n  Error! in Opening '%s'\n", device.c_str() );
////        return;
////    }
////    printf( "\n  '%s' Opened Successfully\n", device.c_str() );
////    if ( tcgetattr( fd, &options ) < 0 )
////    {
////        printf("Failed to get attr!\n");
////    }
////    cfsetispeed( &options, B115200 );
////    cfsetospeed( &options, B115200 );
////    cfmakeraw( &options );
////    // flags
////    //
////    options.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
////    options.c_oflag = 0;
////    options.c_cflag |= CS8 | CREAD | CLOCAL; // 8n1, see termios.h for more information
////    options.c_cflag &= ~(CSIZE | PARENB);
////    options.c_cc[VMIN]  = 1;
////    options.c_cc[VTIME] = 0;
////
//////    options.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
//////    options.c_cflag &= ~PARENB;                         // No Parity
//////    options.c_cflag &= ~CSTOPB;                         // Stop bits = 1
//////    options.c_cflag &= ~CSIZE;                          // Clears the Mask
//////    options.c_cflag |=  CS8;                            // Set the data bits = 8
//////    options.c_cflag |=  CREAD | CLOCAL;                 // Turn on the receiver
//////    options.c_cflag &= ~CRTSCTS;                        // Turn off hardware based flow control (RTS/CTS).
//////    options.c_iflag &= ~(IXON | IXOFF | IXANY);         // Turn off software based flow control (XON/XOFF).
//////    options.c_cc[VMIN] = 0;
//////    options.c_cc[VTIME] = 5;
//////    options.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); // NON Cannonical mode
////
////    if ( tcsetattr( fd, TCSANOW, &options ) < 0 )
////    {
////        printf("Failed to set attr!\n");
////    }
////}
////
////void SerialHandler::writeLine( std::string line )
////{
////    int bytes_written = 0;
////    bytes_written = write( fd, line.c_str(), sizeof(line.c_str()) ); 
////	tcdrain(fd); //waits until all output written to the object referred to by fildes is transmitted
////}
////
////void SerialHandler::closeSerial()
////{
////    close( fd );
////}
