#pragma once
#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <errno.h>  /* ERROR Number Definitions          */
#include <string>

class SerialHandler
{
    private:
    /* use omega UART1 */
    std::string path = "/dev/ttyACM0";
    int fd = -1; 

    public:
    SerialHandler() { }
    virtual ~SerialHandler() { }

    std::string numberToString(unsigned long number);
    void writeNumber( unsigned long number );
    void writeString( std::string s );
    bool openUART( std::string path );
    int set_interface_attribs( int speed, int parity );
    void set_blocking( int should_block );
};
