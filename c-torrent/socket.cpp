
#include "socket.h"

#include <chrono>
#include <thread>


#if defined(OS_WINDOWS)
    unsigned int msock::Socket::_amount = 0;
#endif

msock::Socket::Socket()
{
#if defined(OS_WINDOWS)

    if (0 != _amount++)
        return;

    if ( 0 != WSAStartup(MAKEWORD(2, 2), &_wsaData))
    {
        std::cout << "WSAStartup error: " << GetLastError() << "\n";
    }

#endif
}

void msock::Socket::socket(int family, int sock_type, int protocol)
{
    _socket = ::socket(family, sock_type, protocol);

    if (_socket == INVALID_SOCKET) 
    {
        std::cout << "socket error: " << GetLastError() << "\n";
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
        std::cout << "connect error: " << GetLastError() << "\n";
    }
}

void msock::Socket::connect(sockaddr addr)
{
    int res = 0;

#if defined(OS_WINDOWS)
    res = ::connect(_socket, (SOCKADDR*) &addr, sizeof(addr));
#else
    res = ::connect(_socket, (const struct sockaddr *) &addr, sizeof(addr));
#endif
    if(S_ERROR == res)
    {
        std::cout << "connect error: " << GetLastError() << "\n";
    }
}



void msock::Socket::send(const std::string msg)
{
    int res = 0;

#if defined(OS_WINDOWS)
    res = ::send(_socket, msg.c_str(), msg.size(), 0);
#else
    res = ::write(_socket, msg.c_str(), msg.size());
#endif

    if (S_ERROR == res)
    {
        std::cout << "send error: " << GetLastError() << "\n";
    }
}

void msock::Socket::send(const std::vector<uint8_t> msg)
{
    int res = 0;

#if defined(OS_WINDOWS)
    res = ::send(_socket, reinterpret_cast<const char*>(msg.data()), msg.size(), 0);
#else
    res = ::write(_socket, reinterpret_cast<const char*>(msg.data()), msg.size());
#endif

    if (S_ERROR == res)
    {
        std::cout << "send error: " << GetLastError() << "\n";
    }

}

void msock::Socket::write(const std::string msg)
{
    this->send(msg);
}

void msock::Socket::write(const std::vector<uint8_t> msg)
{
    this->send(msg);
}

std::string msock::Socket::recv()
{
    char buffer[17 * 1024];
    int res = 0;

#if defined(OS_WINDOWS)
    res = ::recv(_socket, buffer, sizeof(buffer), 0);
#else
    res = ::read(_socket, buffer, sizeof(buffer));
#endif

    if (res == 0)
    {
        std::cout << "closed: " << GetLastError() << "\n";
        // closed
    }
    else if (res < 0)
    {
        std::cout << "error: " << GetLastError() << "\n";
        // error
    }

    std::string response(buffer, res);

    return response;
}

std::string msock::Socket::read()
{
    return this->recv();
}

std::vector<uint8_t> msock::Socket::_recv()
{
    char buffer[16 * 1024];
    int res = 0;

#if defined(OS_WINDOWS)
    res = ::recv(_socket, buffer, sizeof(buffer), 0);
#else
    res = ::read(_socket, buffer, sizeof(buffer));
#endif

    if (res == 0)
    {
        std::cout << "closed: " << GetLastError() << "\n";
        // closed
    }
    else if (res < 0)
    {
        std::cout << "error: " << GetLastError() << "\n";
        // error
    }

    std::vector<uint8_t> response((unsigned char)buffer, res);

    return response;
}


void msock::Socket::closesocket()
{
    int res = 0;

#if defined(OS_WINDOWS)
    if (INVALID_SOCKET == _socket)
        return;

    res = ::closesocket(_socket);
#else
    if (-1 == _socket)
        return;

    res = ::close(_socket);
#endif

    if (S_ERROR == res)
    {
        std::cout << "closesocket error: " << GetLastError() << "\n";
    }

#if defined(OS_WINDOWS)
    _socket = INVALID_SOCKET;
#else
    _socket = -1;
#endif
}

void msock::Socket::close()
{
    this->closesocket();
}

msock::Socket::Socket(const Socket & s)
{
#if defined(OS_WINDOWS)
    this->_socket = s._socket;
    this->_wsaData = s._wsaData;
    ++_amount;
#else
    this->_socket = s._socket;

#endif
}


msock::Socket::~Socket()
{
    if (_socket != INVALID_SOCKET)
        closesocket();

#if defined(OS_WINDOWS)
    if (0 == _amount)
    {
        WSACleanup();
        return;
    }

    --_amount;
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

void msock::sleep(const int64_t time)
{
   std::this_thread::sleep_for(std::chrono::milliseconds(time)); 
}