
#include "peer.h"
#include "hash.h"

#include <sstream>

Peer::Peer(std::vector<int> ip_port)
{
    if (ip_port.size() < 5)
        return;

    ip[0] = ip_port[0];
    ip[1] = ip_port[1];
    ip[2] = ip_port[2];
    ip[3] = ip_port[3];
    port = ip_port[4];
}

void Peer::connect()
{
    sock.socket(AF_INET, SOCK_STREAM, 0);

    auto peer_ip = std::to_string(ip[0]) + "." + std::to_string(ip[1]) + "." +
        std::to_string(ip[2]) + "." + std::to_string(ip[3]);

    struct sockaddr_in saddr; 

    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = msock::Socket::inet_addr(peer_ip.c_str());
    saddr.sin_port = htons(port);

    sock.connect(saddr);
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

std::string Peer::request_get_peer_id(MetaInfo minfo)
{
    using namespace std::string_literals;
    // Create message
    std::string handshake_message = std::string("\x13") +                // length of the protocol string
                                    "BitTorrent protocol"s +              // protocol string
                                    "\x00\x00\x00\x00\x00\x00\x00\x00"s + // reserved
                                    get_info_string(calculate_info_hash(minfo)) +         // info hash
                                    "00112233445566778899"s;              // peer id

    // Send message

    sock.send(handshake_message);

    auto response = sock.recv();

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

bool Peer::operator==(const Peer& peer) const
{
    return this->ip[0] == peer.ip[0] &&
        this->ip[1] == peer.ip[1] &&
        this->ip[2] == peer.ip[2] &&
        this->ip[3] == peer.ip[3] &&
        this->port == peer.port;
}

std::ostream& operator<<( std::ostream& out, const Peer& peer)
{
    return out << peer.ip[0] << "." << peer.ip[1] << "." <<
        peer.ip[2] << "." << peer.ip[3] << ":" << peer.port;
}

