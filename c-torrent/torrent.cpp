
#include "torrent.h"
#include "bencoder.h"
#include "logger.h"

#include "async_writer.h"

//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib/httplib.h"
#include "socket.h"

#include "hash.h" 

#include <iostream>
#include <fstream>
#include <random>
#include <stack>
#include <thread>
#include <future>

std::tuple<std::string, std::string> split_domain_and_endpoint(const std::string& tracker_url) 
{
    LOG
    auto last_forward_slash_index = tracker_url.find_last_of('/');
    auto first_forward_slash_index = tracker_url.find_first_of('/');

    if (first_forward_slash_index == last_forward_slash_index - 1)
        return {tracker_url, ""};

    auto domain = tracker_url.substr(0, last_forward_slash_index);
    auto endpoint = tracker_url.substr(last_forward_slash_index, tracker_url.size() - last_forward_slash_index);

    _log(I) << "domain: " << domain;
    _log(I) << "endpoint: " << endpoint;

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

std::vector<Peer> decode_peers(std::string& encoded_peers) 
{
    std::vector<Peer> peers{};

    for(auto i = 0; i < encoded_peers.size(); i += 6) 
    {
        auto str_peer = encoded_peers.substr(i, 6);

        Peer peer;

        // First four bytes are the ip, the final two bytes are the port in big endian.
        peer.ip[0] = static_cast<uint8_t>(str_peer[0]),
        peer.ip[1] = static_cast<uint8_t>(str_peer[1]),
        peer.ip[2] = static_cast<uint8_t>(str_peer[2]),
        peer.ip[3] = static_cast<uint8_t>(str_peer[3]),
        peer.port = static_cast<uint8_t>(str_peer[4]) << 8 | static_cast<uint8_t>(str_peer[5]);

        peers.push_back(peer);
    }
    
    return peers;
}

//#define CPPHTTPLIB_OPENSSL_SUPPORT

std::vector<Peer> BitTorrent::request_get_peers()
{
    LOG
    if (std::string::npos != minfo.announce.find("http"))
    {
        _log(I) << "http";
        auto domain_and_endpoint = split_domain_and_endpoint(minfo.announce);
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

        httplib::Client cli(std::get<0>(domain_and_endpoint));

        auto res = cli.Get(
            std::get<1>(domain_and_endpoint) + "?info_hash=" + 
                encode_info_hash(calculate_info_hash(minfo)),
            params,
            headers
        );

        if (httplib::Error::Success != res.error())
        {
            std::cout << httplib::to_string(res.error()) << "\n";
            return {};
        }

        auto str = Bencoder::decode(res->body);
        
        auto it = std::find(str.begin(), str.end(), "peers");

        // https://wiki.theory.org/BitTorrentSpecification#Tracker_HTTP.2FHTTPS_Protocol

        // тут добавить получение interval-ов между запросами

        if (str.end() == it)
            return {};

        if (str.end() == ++it)
            return {};

        return decode_peers(*it);
    }
    if (std::string::npos != minfo.announce.find("udp"))
    {
        _log(I) << "udp";
        // https://www.bittorrent.org/beps/bep_0015.html

        auto colon_index = minfo.announce.find_last_of(":");
        auto url = minfo.announce.substr(0, colon_index);
        auto port = minfo.announce.substr(colon_index + 1, minfo.announce.size() - colon_index);
        std::string scheme;
        int res = 0;

        msock::Socket socket;

        addrinfo* result = nullptr;
        addrinfo hints;


        url += "/" ;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC; //AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_UDP;

        system("pause");

        res = getaddrinfo(minfo.announce.c_str(), NULL, &hints, &result);
        if (0 != res)
        {
            std::cout << "Error1: "<< res <<   "\n";
            std::cout << socket.GetLastError();
        }

        int i = 0;
        struct addrinfo *ptr = NULL;
        for(ptr=result; ptr != NULL ;ptr=ptr->ai_next)
        {
            printf("getaddrinfo response %d\n", i++);
            printf("\tFlags: 0x%x\n", ptr->ai_flags);
            printf("\tFamily: ");
        }

        /*
        socket.socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (INVALID_SOCKET == socket)
        {
            std::cout << "Error2";
        }

        socket.connect(*(result->ai_addr));
        //if (iResult == SOCKET_ERROR) 
        */



        std::vector<uint8_t> msg;

        uint64_t protocol_id = 0x41727101980;
        const uint8_t* protocol_id_bytes = reinterpret_cast<const uint8_t *>(&protocol_id);
        msg.insert(msg.end(), protocol_id_bytes, protocol_id_bytes + sizeof(protocol_id));

        uint32_t action = 0;
        const uint8_t* action_bytes = reinterpret_cast<const uint8_t *>(&action_bytes);
        msg.insert(msg.end(), action_bytes, action_bytes + sizeof(action));

        uint32_t transaction = 0;
        const uint8_t* transaction_bytes = reinterpret_cast<const uint8_t *>(&transaction);
        msg.insert(msg.end(), transaction_bytes, transaction_bytes + sizeof(transaction));



        socket.send(msg);

        auto str = socket.recv();

        std::cout << str;
        
        /*
        addrinfo* result = nullptr;
        addrinfo hints = {0};

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        getaddrinfo(url.c_str(), port.c_str(), &hints, &result);


        char *hostAddress;
        struct sockaddr_in *addr;
        struct addrinfo *rp;
        for (rp = result; rp != NULL; rp = rp->ai_next) {
            addr = (struct sockaddr_in *)rp->ai_addr; 
            printf("dstPort  = %d\n",ntohs(addr->sin_port));
            printf("dstAddr  = %s\n",inet_ntoa((struct in_addr)addr->sin_addr));
            hostAddress = inet_ntoa((struct in_addr)addr->sin_addr);    
        }

        socket.socket(result->ai_family, result->ai_socktype, result->ai_protocol);

        socket.connect(*(result->ai_addr));

        std::string request = std::string("GET") 
            + " / "
            + "HTTP/1.1"
            + "\r\n"
            + "Host: " 
            + url
            + "\r\n"
            + "\r\n"
            + "?info_hash= "
            + encode_info_hash(calculate_info_hash(minfo))
            + "peer_id= "
            + "00112233445566778899" 
            + "port= " 
            + port
            + "uploaded= "
            + "0"
            + "downloaded= "
            + "0"
            + "left= "
            + minfo.info.length
            + "compact= "
            + "1"
        ;

        socket.send(request);

        auto str = socket.recv();

        std::cout << str << "\n";
        */
    }

    return {};
}


#include <bitset>
#include <iostream>
void BitTorrent::download (std::string file_name)
{
    LOG
    auto peers = request_get_peers();

    if (peers.empty())
        return;

    auto peer = peers.begin();

    std::string peer_id;
    do 
    {
        peer->connect();
        peer_id = peer->request_get_peer_id(minfo);

    } while(peer_id.empty() && peers.end() != peer);

    if (peers.end() == peer)
    {
        _log(W) << "Requested peer id failed";
        std::cout << "Requested peer id failed";
        return;
    }

    peer->send_interested();

    AsyncWriter file(file_name);

    auto pieces = get_pieces(minfo.info._pieces);
    for (int i = 0; i < pieces.size() && peer != peers.end(); ++i)
    {
        std::cout << *peer << "\n";
        auto future = std::async(&Peer::download_piece, peer, std::ref(file), minfo, i);
        //auto future = std::async(peer->download_piece, file, minfo, i);

        if (future.get())
            continue;

        if (peers.end() != ++peer)
        {
            std::cout << *peer << "\n";
            peer->connect();
            if (peer->request_get_peer_id(minfo).empty())
            {
                //system("pause");
                //some problems
                _log(W) << "Requested peer id failed";
                std::cout << "some problems";
                return;
            }
            peer->send_interested();
            --i;
        }
    }

}