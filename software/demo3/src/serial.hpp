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
    const char *portname = "/dev/ttyACM0";
    int uartFd = -1; 

    public:
    SerialHandler() { }
    virtual ~SerialHandler() { }

    int set_interface_attribs (int fd, int speed, int parity);
    void set_blocking (int fd, int should_block);
    void uart_writestr(const char* string);
    void uart_write(void* data, size_t len);
    ssize_t uart_read(void* buffer, size_t charsToRead);
    int uart_open(const char* port, int baud, int blocking);
    void uart_close();

//    void openSerial( std::string );
//    void writeLine( std::string );
//    void closeSerial( );
};

//#include <iostream>
//
//#ifdef __linux__
//#define DEVICE_PORT "/dev/ttyO1"
//#endif
//
//int main()
//{
//    std::serialib serial; //the main class to access
//    int return_value;
//
//    return_value = serial.Open( DEVICE_PORT, 9600 );
//    if ( return_value != 1 )
//    {
//        printf("cant open port\n");
//        return 0;
//    }
//    else
//    {
//        printf("port now open \n");
//    }
//
//    string x = "650";
//    string y = "650";
//
//    for ( int i = 0; i < 500; i++ )//send x 500 times 
//    {
//        for ( int j = 0; j < x.length(); j++ ) //send each character separately 
//        {
//            return_value = serial.WriteChar(x[j]);
//            serial.Close();
//            serial.Open(DEVICE_PORT,300);
//        }
//        if ( return_value != 1 )
//        {
//            printf("cannot write\n");
//        }
//        else 
//        {
//            printf("done writing\n");
//        }
//    }
//    serial.Close();
//    cout<<"Transmission complete\n";
//}
