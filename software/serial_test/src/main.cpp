#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "serial.hpp"

int main(int, char**)
{
    SerialHandler sh;

    std::string path = "/dev/ttyACM0";
    if ( sh.openUART( path ) == false )
    {
        printf( "Error opening '%s'\n", path.c_str() );
        return -1;
    }

    sh.set_interface_attribs( B115200, 0 );   // set speed to 115,200 bps, 8n1 (no parity)
    sh.set_blocking( 0 );                     // set no blocking

    sh.writeNumber( 1234 );                   // send 7 character greeting
    usleep( 10000 );                          // sleep enough to transmit the 7 plus
    sh.writeNumber( 5678 );                   // send 7 character greeting
    usleep( 10000 );                          // sleep enough to transmit the 7 plus
    sh.writeNumber( 9876 );                   // send 7 character greeting
    usleep( 10000 );                          // sleep enough to transmit the 7 plus
    sh.writeNumber( 5432 );                   // send 7 character greeting
    usleep( 10000 );                          // sleep enough to transmit the 7 plus
    sh.writeString( "stop\r" );               // send 7 character greeting
    usleep( 10000 );                          // sleep enough to transmit the 7 plus
                                              // receive 25:  approx 100 uS per char transmit
//    char buf [100];
//    int n = read( fd, buf, sizeof(buf) );  // read up to 100 characters if ready to read

    return 0;
}
