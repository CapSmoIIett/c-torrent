
#include "torrent.h"
#include "hash.h" 
#include "../c-torrent/bencoder.h"

#include "httplib/httplib.h"

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


std::vector<Peer> BitTorrent::request_get_peers()
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

    if (httplib::Error::Success != res.error())
    {
        std::cout << httplib::to_string(res.error()) << "\n";
        return {};
    }

    auto str = Bencoder::decode(res->body);
    
    auto it = std::find(str.begin(), str.end(), "peers");

    if (str.end() == it)
        return {};

    if (str.end() == ++it)
        return {};

    return decode_peers(*it);

}

std::vector<uint8_t> create_msg (const uint8_t type, std::string payload = std::string())
{
    // const uint8_t msg_ID = 2;
    std::vector<uint8_t> msg;
    std::vector<uint8_t> _payload (payload.begin(), payload.end());

    uint32_t total_length = 4                 // message length (4 bytes)
                            + 1               // message ID (1 byte)
                            + _payload.size(); // payload length (variable)

    msg.reserve(total_length);

    // actual message length
    uint32_t networkLength = htonl(total_length - 4);

    const uint8_t *lengthBytes = reinterpret_cast<const uint8_t *>(&networkLength);

    // write length of msg
    msg.insert(msg.end(), lengthBytes, lengthBytes + sizeof(networkLength));
    // write message id
    msg.push_back(static_cast<uint8_t>(type));
    msg.insert(msg.end(), _payload.begin(), _payload.end());

    return msg;
}


std::string BitTorrent::download_piece()
{
    auto arr = request_get_peers();

    auto& peer = arr[0];
    auto msg = create_msg(INTERESTED);

    std::cout << (char*)msg.data() << "\n";

    peer.connect();
    auto& sock = peer.sock;

    peer.request_get_peer_id(minfo);

    auto res = sock.recv();
    std::cout << res << "\n";
    std::cout << "size: " << get_msg_size(res) << "\n";
    std::cout << "type: " << get_msg_type(res) << "\n";

    sock.send(msg);

    res = sock.recv();
    std::cout << res << "\n";
    std::cout << "size: " << get_msg_size(res) << "\n";
    std::cout << "type: " << get_msg_type(res) << "\n";

    return "";
}
