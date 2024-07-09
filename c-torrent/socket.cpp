
#include "socket.h"

msock::Socket::Socket()
{
#if defined(OS_WINDOWS)

    if ( 0 != WSAStartup(MAKEWORD(2, 2), &_wsaData))
    {
        std::cout << "WSAStartup error " << GetLastError() << "\n";
    }

    //if ( 0 != )

#endif
}

void msock::Socket::socket()
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


void msock::Socket::connect(sockaddr_in addr)
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



void msock::Socket::send(std::string msg)
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

void msock::Socket::write(std::string msg)
{
    this->send(msg);
}

std::string msock::Socket::recv()
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

std::string msock::Socket::read()
{
    return this->recv();
}


void msock::Socket::closesocket()
{
    int res = 0;

#if defined(OS_WINDOWS)
    if (INVALID_SOCKET != _socket)
        res = ::closesocket(_socket);
#else
    if (-1 == _socket)
        res = ::close(_socket);
#endif

    if (S_ERROR == res)
    {
        std::cout << "closesocket error" << GetLastError() << "\n";
    }

    _socket = INVALID_SOCKET;
}

void msock::Socket::close()
{
    this->closesocket();
}



msock::Socket::~Socket()
{
    if (_socket != INVALID_SOCKET)
        closesocket();

#if defined(OS_WINDOWS)
    WSACleanup();
#endif
}


#if defined(OS_WINDOWS)
    unsigned long msock::Socket::inet_addr(const char *ip)
    {
        return ::inet_addr(ip);
    }
#else
    in_addr_t msock::Socket::inet_addr(const char *ip)
    {

        struct hostent* host = gethostbyname(ip);
        return ::inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    }
#endif


std::string msock::Socket::GetLastError()
{
    std::string answer;

#if defined(OS_WINDOWS)
    DWORD err_id = ::GetLastError();

    //No error message has been recorded
    if(err_id == 0) 
        return std::string(); 
    
    LPSTR messageBuffer = nullptr;

    //Ask Win32 to give us the string version of that message ID.
    //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, err_id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
    
    //Copy the error message into a std::string.
    answer = std::string(messageBuffer, size);
    
    //Free the Win32's string's buffer.
    LocalFree(messageBuffer);

#else
    answer = " ";
#endif

    return answer;
}