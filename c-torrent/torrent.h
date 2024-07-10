#pragma once

#include "metainfo.h"
#include <vector>
#include <string>

//#include "socket.h"
#include "peer.h"


#define CHOKE       0
#define UNCHOKE     1
#define INTERESTED  2
#define NOT_INTERESTED 3
#define HAVE        4
#define BITFIELD    5
#define REQUEST     6
#define PIECE       7
#define CANCEL      8

class BitTorrent
{
public:
    BitTorrent() = delete;
    BitTorrent(MetaInfo info) :
        minfo(info)
    { }

    std::vector<Peer> request_get_peers();
    
    std::string download_piece();

    MetaInfo minfo;

private:
    std::vector<Peer> peers = { };

};


inline std::vector<uint8_t> str_to_bytes(std::string str)
{
    return std::vector<uint8_t>(str.begin(), str.end());
}

inline uint32_t get_msg_size(std::vector<uint8_t> msg)
{
    return (msg[0] << 24) + (msg[1] << 16) + (msg[2] << 8) + msg[3];
}

inline uint32_t get_msg_size(std::string msg)
{
    return get_msg_size(str_to_bytes(msg));
}

inline uint8_t get_msg_type(std::vector<uint8_t> msg)
{
    // uint8_t to int
    return static_cast<int>(msg[4]);
}

inline int get_msg_type(std::string msg)
{
    return get_msg_type(str_to_bytes(msg));
}
