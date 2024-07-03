#pragma once


#if defined(_WIN32) || defined(_WIN64)
	#define OS_WINDOWS
#elif defined(__unix__) || defined(__unix)
	#define OS_POSIX
#else
	#error unsupported platform
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

#endif

#include <string>
#include <memory>
#include <iostream>


class Socket final 
{
public:

    Socket();
    ~Socket();

    void socket();

    void connect(sockaddr_in);

    void send(std::string);
    void write(std::string);

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

    struct wsadata_deleter {
				void operator()(WSADATA* other) {
					WSACleanup();
					delete other;
				}
			};

    SOCKET _socket = INVALID_SOCKET;
    std::unique_ptr<WSADATA, wsadata_deleter> _wsaData = {new WSADATA, wsadata_deleter()};
#else
    int _socket = -1;

#endif

};


Socket::Socket()
{
#if defined(OS_WINDOWS)

    if ( 0 != WSAStartup(MAKEWORD(2, 2), _wsaData.get()))
    {
        std::cout << "WSAStartup error " << GetLastError() << "\n";
    }

    //if ( 0 != )

#endif

}


void Socket::socket()
{
    _socket = ::socket(AF_INET, SOCK_STREAM, 0);

    if (_socket == INVALID_SOCKET) 
    {
        std::cout << "socket error ";

        #if defined(OS_WINDOWS)
            std::cout << GetLastError();
        #endif

        std::cout << "\n";
    }
}


void Socket::connect(sockaddr_in addr)
{
    int res = 0;

#if defined(OS_WINDOWS)
    res = ::connect(_socket, (SOCKADDR*) &addr, sizeof(addr));
#else
    res = ::connect(_socket, (const struct sockaddr *) &addr, sizeof(addr));
#endif
    if(S_ERROR == res)
    {
        std::cout << "connect error " << GetLastError() << "\n";
    }
}


void Socket::send(std::string msg)
{
    int res = 0;

#if defined(OS_WINDOWS)
    res = ::send(_socket, msg.c_str(), msg.size(), 0);
#else
    res = ::write(_socket, msg.c_str(), msg.size());
#endif

    if (S_ERROR == res)
    {
        std::cout << "send error " << GetLastError() << "\n";
    }
}

void Socket::write(std::string msg)
{
    this->send(msg);
}

std::string Socket::recv()
{
    char buffer[1024];
    int res = 0;

#if defined(OS_WINDOWS)
    res = ::recv(_socket, buffer, sizeof(buffer), 0);
#else
    res = ::read(_socket, buffer, sizeof(buffer));
#endif

    if (res == 0)
    {
        std::cout << "closed " << GetLastError() << "\n";
        // closed
    }
    else if (res < 0)
    {
        std::cout << "error " << GetLastError() << "\n";
        // error
    }

    std::string response(buffer, res);

    return response;
}

std::string Socket::read()
{
    return this->recv();
}


void Socket::closesocket()
{
    int res = 0;

#if defined(OS_WINDOWS)
    res = ::closesocket(_socket);
#else
    res = ::close(_socket);
#endif

    if (S_ERROR == res)
    {
        std::cout << "closesocket error" << GetLastError() << "\n";
    }
}

void Socket::close()
{
    this->closesocket();
}

std::string Socket::GetLastError()
{
    std::string answer;

#if defined(OS_WINDOWS)
    answer = WSAGetLastError();
#else
    answer = " ";
#endif

    return answer;
}



Socket::~Socket()
{
#if defined(OS_WINDOWS)
    WSACleanup();
#endif
}


#if defined(OS_WINDOWS)
    unsigned long Socket::inet_addr(const char *ip)
    {
        return ::inet_addr(ip);
    }
#else
    in_addr_t Socket::inet_addr(const char *ip)
    {

        struct hostent* host = gethostbyname(ip);
        return ::inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    }
#endif

