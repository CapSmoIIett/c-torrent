#pragma once

#if defined(OS_WINDOWS)

class AutoWSA 
{
public: 
    static init()
    {
        static AutoWSA wsa;
    }

    static WSADATA data()
    {
        return _wsaData;
    }

    AutoWSA

private:
    WSADATA _wsaData;

    AutoWSA()
    {
        if ( 0 != WSAStartup(MAKEWORD(2, 2), &_wsaData))
        {
            std::cout << "WSAStartup error: " << GetLastError() << "\n";
        }
    }

    ~AutoWSA()
    {
        WSACleanup();
    }

}

#endif