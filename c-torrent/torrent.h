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

#define KB          1024

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

inline uint32_t get_msg_piece_index(std::vector<uint8_t> msg)
{
    return (msg[5] << 24) + (msg[6] << 16) + (msg[7] << 8) + msg[8];
}

inline uint32_t get_msg_piece_index(std::string msg)
{
    return get_msg_piece_index(str_to_bytes(msg));
}

inline std::vector<uint8_t> get_msg_piece(std::vector<uint8_t> msg)
{
    size_t header_size = 
        1 + // type
        4 + // index
        4;  // offset

    return std::vector<uint8_t>(msg.begin() + header_size, msg.end());
}

inline std::vector<uint8_t> create_payload_request(const uint32_t index, const uint32_t begin, const uint32_t length)
{
    std::vector<uint8_t> payload;

    // write piece index

    uint32_t _index = htonl(index);
    const uint8_t *index_bytes = reinterpret_cast<const uint8_t *>(&_index);
    payload.insert(payload.end(), index_bytes, index_bytes + sizeof(_index));
    
    uint32_t _begin = htonl(begin);
    const uint8_t *begin_bytes = reinterpret_cast<const uint8_t *>(&_begin);
    payload.insert(payload.end(), begin_bytes, begin_bytes + sizeof(_begin));

    uint32_t _length = htonl(length);
    const uint8_t *length_bytes = reinterpret_cast<const uint8_t *>(&_length);
    payload.insert(payload.end(), length_bytes, length_bytes + sizeof(_length));

    return payload;
}
