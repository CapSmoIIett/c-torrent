
#define CHOKE       0
#define UNCHOKE     1
#define INTERESTED  2
#define NOT_INTERESTED 3
#define HAVE        4
#define BITFIELD    5
#define REQUEST     6
#define PIECE       7
#define CANCEL      8
#define PORT        9
#define HAVE_ALL    14
#define HAVE_NONE   15
#define REJECT_REQUEST 16
#define EXTENDED    20

#define KB          1024

#include <vector>
#include <string>

inline std::vector<uint8_t> str_to_bytes(const std::string& str)
{
    return std::vector<uint8_t>(str.begin(), str.end());
}

inline uint32_t get_msg_size(const std::vector<uint8_t>& msg)
{
    if (4 > msg.size())
        //error
        return -1;
    return (msg[0] << 24) + (msg[1] << 16) + (msg[2] << 8) + msg[3];
}

inline uint32_t get_msg_size(const std::string& msg)
{
    return get_msg_size(str_to_bytes(msg));
}

inline uint8_t get_msg_type(const std::vector<uint8_t>& msg)
{
    // uint8_t to int
    if (5 > msg.size())
        //error
        return -1;

    return static_cast<int>(msg[4]);
}

inline int get_msg_type(const std::string& msg)
{
    return get_msg_type(str_to_bytes(msg));
}

inline uint32_t get_msg_piece_index(const std::vector<uint8_t>& msg)
{
    if (9 > msg.size())
        //error
        return -1;

    return (msg[5] << 24) + (msg[6] << 16) + (msg[7] << 8) + msg[8];
}

inline uint32_t get_msg_piece_index(const std::string& msg)
{
    return get_msg_piece_index(str_to_bytes(msg));
}

inline std::vector<uint8_t> get_msg_piece(const std::vector<uint8_t>& msg)
{
    size_t header_size = 
        4 + // size
        1 + // type
        4 + // index
        4;  // offset

    if (header_size >= msg.size())
        // error
        return {};

    return std::vector<uint8_t>(msg.begin() + header_size, msg.end());
}

inline std::string get_msg_piece(const std::string& msg)
{
    size_t header_size = 
        4 + // size
        1 + // type
        4 + // index
        4;  // offset

    if (header_size >= msg.size())
        // error
        return "";

    return std::string(msg.begin() + header_size, msg.end());
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

inline std::vector<uint8_t> create_msg (const uint8_t type, std::vector<uint8_t> payload = {})
{
    // const uint8_t msg_ID = 2;
    std::vector<uint8_t> msg;

    uint32_t total_length = 4                 // message length (4 bytes)
                            + 1               // message ID (1 byte)
                            + payload.size(); // payload length (variable)

    msg.reserve(total_length);

    // actual message length
    uint32_t networkLength = htonl(total_length - 4);

    // write length of msg
    const uint8_t *lengthBytes = reinterpret_cast<const uint8_t *>(&networkLength);
    msg.insert(msg.end(), lengthBytes, lengthBytes + sizeof(networkLength));

    // write message id
    msg.push_back(static_cast<uint8_t>(type));
    msg.insert(msg.end(), payload.begin(), payload.end());

    return msg;
}