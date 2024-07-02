
#include "request.h"
//#include "HTTPRequest/include/HTTPRequest.hpp"
#include "httplib/httplib.h"
#include "../c-torrent/bencoder.h"

#include "hash.h" 
#include <tuple>


#include <iostream>

std::tuple<std::string, std::string> split_domain_and_endpoint(const std::string& tracker_url) 
{
    auto last_forward_slash_index = tracker_url.find_last_of('/');
    auto first_forward_slash_index = tracker_url.find_first_of('/');

    if (first_forward_slash_index == last_forward_slash_index - 1)
        return {tracker_url, ""};

    auto domain = tracker_url.substr(0, last_forward_slash_index);
    auto endpoint = tracker_url.substr(last_forward_slash_index, tracker_url.size() - last_forward_slash_index);

    return std::make_tuple(domain, endpoint);
}

std::string encode_info_hash(const std::string& hash) 
{
    std::string encoded;
    for(auto i = 0; i < hash.size(); i += 2) 
    {
        encoded += '%' + hash.substr(i, 2);
    }
    return encoded;
}

std::vector<std::vector<long long int>> decode_peers(std::string& encoded_peers) 
{
    std::vector<std::vector<long long int>> peers{};

    for(auto i = 0; i < encoded_peers.size(); i += 6) 
    {
        auto peer = encoded_peers.substr(i, 6);
        // First four bytes are the ip, the final two bytes are the port in big endian.
        peers.push_back(
            {
                static_cast<uint8_t>(peer[0]),
                static_cast<uint8_t>(peer[1]),
                static_cast<uint8_t>(peer[2]),
                static_cast<uint8_t>(peer[3]),
                static_cast<uint8_t>(peer[4]) << 8 | static_cast<uint8_t>(peer[5])
            });
    }
    
    return peers;
}

std::vector<std::vector<long long int>> request_get_nodes(const MetaInfo minfo)
{
    size_t port = 6881;
    size_t uploaded = 0;
    size_t downloaded = 0;
    size_t compact = 1;

    httplib::Params params{
        {"peer_id", "00112233445566778899"},
        { "port", std::to_string(port)},
        {"uploaded", "0"},
        {"downloaded", "0"},
        { "left", minfo.info.length},
        {"compact", "1"}
    };

    httplib::Headers headers{};

    auto domain_and_endpoint = split_domain_and_endpoint(minfo.announce);

    //httplib::Client cli(minfo.announce);
    //auto res = cli.Get("", headers);

    httplib::Client cli(std::get<0>(domain_and_endpoint));
    auto res = cli.Get(
        std::get<1>(domain_and_endpoint) + "?info_hash=" + 
            encode_info_hash(calculate_info_hash(minfo)),
        params,
        headers
    );

    auto str = Bencoder::decode(res->body);
    
    auto it = std::find(str.begin(), str.end(), "peers");

    if (str.end() == it)
        return {};

    if (str.end() == ++it)
        return {};

    return decode_peers(*it);

}

#if defined(_WIN32) || defined(_WIN64)
	#define OS_WINDOWS
#elif defined(__unix__) || defined(__unix)
	#define OS_POSIX
#else
	#error unsupported platform
#endif


#ifdef OS_WINDOWS	 
#define WIN(exp) exp
#define NIX(exp)

#else
#define WIN(exp)
#define NIX(exp) exp
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

std::string get_info_string(std::string info_hash)
{
    std::vector<uint8_t> bytes;
    for (unsigned int i = 0; i < info_hash.length(); i += 2)
    {
        std::string byteString = info_hash.substr(i, 2);
        char byte = (char)strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }
    std::string info_string = std::string(reinterpret_cast<char *>(bytes.data()), bytes.size() * sizeof(uint8_t));
    return info_string;
}


std::string request_get_peer_id(const MetaInfo minfo, std::string peer_ip, std::string peer_port)
{
    using namespace std::string_literals;
    // Create message
    std::string handshake_message = std::string("\x13") +                // length of the protocol string
                                    "BitTorrent protocol"s +              // protocol string
                                    "\x00\x00\x00\x00\x00\x00\x00\x00"s + // reserved
                                    get_info_string(calculate_info_hash(minfo)) +         // info hash
                                    "00112233445566778899"s;              // peer id

    // Send message

    char buffer[1024];

#if defined(OS_WINDOWS)
    WSADATA wsaData = {0};
    SOCKET sock = INVALID_SOCKET;
    int res = 0;

    if ( 0 != WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        std::cout << "WSAStartup error " << WSAGetLastError() << "\n";
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) 
    {
        std::cout << "socket error " << WSAGetLastError() << "\n";
    }

    struct sockaddr_in saddr; 
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(peer_ip.c_str());
    saddr.sin_port = htons(std::stoi(peer_port));

    if(SOCKET_ERROR == connect(sock, (SOCKADDR*) &saddr, sizeof(saddr)))
    {
        std::cout << "connect error " << WSAGetLastError() << "\n";
    }

    res = send(sock, handshake_message.c_str(), handshake_message.size(), 0);
    if (SOCKET_ERROR == res)
    {
        std::cout << "send error " << WSAGetLastError() << "\n";
    }

    res = recv(sock, buffer, sizeof(buffer), 0);
    if (res == 0)
    {
        std::cout << "closed " << WSAGetLastError() << "\n";
        // closed
    }
    else if (res < 0)
    {
        std::cout << "error " << WSAGetLastError() << "\n";
        // error
    }

    //*/
    std::string response(buffer, res);

    res = closesocket(sock);
    if (SOCKET_ERROR == res)
    {
        std::cout << "closesocket error" << WSAGetLastError() << "\n";
    }

    WSACleanup();

#else

#endif

    size_t start_pos =
        28    // length of the protocol string + reserved bytes
        + 20; // length of the info hash
    size_t peer_id_length = 20;

    std::string peer_id_raw = response.substr(start_pos, peer_id_length);
    // get peer_id in hex format
    std::stringstream ss;
    for (unsigned char c : peer_id_raw)
    {
        ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(c);
    }

    return ss.str();

}