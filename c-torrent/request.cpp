
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


#include "socket.h"

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

    Socket socket;

    int res = 0;

    socket.socket();

    struct sockaddr_in saddr; 
//  struct sockaddr_in addr;

    saddr.sin_family = AF_INET;

    saddr.sin_addr.s_addr = Socket::inet_addr(peer_ip.c_str());
//  struct hostent* host = gethostbyname(peer_ip.c_str());
//  addr.sin_addr.s_addr = 
//      inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));

    saddr.sin_port = htons(std::stoi(peer_port));

    socket.connect(saddr);

    socket.send(handshake_message);

    auto response = socket.recv();

    //*/

    socket.closesocket();


/*

    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(std::stoi(peer_port));
    */


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

void download_piece_command(std::string tfile, std::string outfile, int piece_index)
{
    TFileParser parser;
    parser.parse_file(TFileParser::open_file(tfile));

    auto minfo = get_meta_info(parser);


    auto arr = request_get_nodes(minfo);

    auto peer_ip = std::to_string(arr[0][0]) + "." + std::to_string(arr[0][1]) + "." +
        std::to_string(arr[0][2]) + "." + std::to_string(arr[0][3]);

    auto peer_port = std::to_string(arr[0][4]);

    auto peer_id = request_get_peer_id(minfo, peer_ip, peer_port);


    //

    // const uint8_t INTERESTED_MESSAGE_ID = 2;
    std::vector<uint8_t> msg;
    std::vector<uint8_t> payload;
    uint32_t total_length = 4                 // message length (4 bytes)
                            + 1               // message ID (1 byte)
                            + payload.size(); // payload length (variable)
    msg.reserve(total_length);
    // actual message length
    uint32_t networkLength = htonl(total_length - 4);
    const uint8_t *lengthBytes = reinterpret_cast<const uint8_t *>(&networkLength);
    msg.insert(msg.end(), lengthBytes, lengthBytes + sizeof(networkLength));
    msg.push_back(static_cast<uint8_t>(2));
    msg.insert(msg.end(), payload.begin(), payload.end());
}