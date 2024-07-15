
#pragma once

#if defined(_WIN32) || defined(_WIN64)
	#define OS_WINDOWS
#elif defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
	#define OS_POSIX
#else
	#error unsupported platform
#endif

#ifndef INVALID_SOCKET
    #define INVALID_SOCKET -1
#endif

#if defined(OS_WINDOWS)
	//#include <Windows.h>
	#include <WinSock2.h> 
	#include <WS2tcpip.h>

#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <unistd.h>
	#include <stdio.h>
	#include <stdlib.h>

    #include <arpa/inet.h>
    #include <errno.h>
    #include <netdb.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <unistd.h>
#endif

#include <string>
#include <vector>
#include <memory>
#include <iostream>

namespace msock
{

class Socket final 
{
public:

    Socket();
    ~Socket();

    Socket(const Socket &);

    void socket(int family, int sock_type, int protocol);

    void connect(sockaddr_in);
    void connect(sockaddr);

    void send(const std::string);
    void send(const std::vector<uint8_t>);

    void write(const std::string);
    void write(const std::vector<uint8_t>);

    std::string recv();
    std::string read();

    void closesocket();
    void close();

    std::string GetLastError();


#if defined(OS_WINDOWS)
    static const long S_ERROR = SOCKET_ERROR;
#else
    static const long S_ERROR = SO_ERROR;
#endif
    

#if defined(OS_WINDOWS)
    static unsigned long inet_addr(const char *ip);
#else
    static in_addr_t inet_addr(const char *ip);
#endif



private:
#if defined(OS_WINDOWS)
    SOCKET _socket = INVALID_SOCKET;
    WSADATA _wsaData;

    // static inline unsigned int _amount;  // only for c++17
    static unsigned int _amount;
#else
    int _socket = -1;

#endif

};


}