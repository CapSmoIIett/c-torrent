
#include "peer.h"
#include "hash.h"
#include "request_helper.h"

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

void Peer::disconect()
{
    sock.closesocket();
}

bool Peer::connected()
{
    return sock.connected();
}

void Peer::send_interested()
{
    auto msg = create_msg(INTERESTED);
    sock.send(msg);

    auto res = sock.recv();
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

    if (response.empty())
        return std::string();

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

#include <iostream>
bool Peer::download_piece(AsyncWriter& file, MetaInfo minfo, int piece_num)
{
    auto piece_hash = get_pieces(minfo.info._pieces)[piece_num];
    std::string hash;   // hash of result string
    size_t piece_length = std::stoi(minfo.info._piece_length);
    size_t file_length = std::stoi(minfo.info.length);

    // amount pieces in file
    int amount_pieces = file_length / piece_length;
    
    // normal piece size
    size_t piece_size = piece_num != amount_pieces ? piece_length : file_length % piece_length; 

    // we download the piece in two halves
    size_t first_half_size = piece_length / 2;
    size_t second_half_size = piece_size - first_half_size > 0 ? piece_size - first_half_size : 0;

    std::string piece;

    int timeout = 100;

    int attempt = 0;
    const int max_attempts = 3;
    do
    {
        // before each attempt clear result string
        piece = std::string();

        if (piece_size > first_half_size)
        {
            auto msg1 = create_msg(REQUEST, create_payload_request(piece_num, 0, first_half_size));
            auto msg2 = create_msg(REQUEST, create_payload_request(piece_num, first_half_size, second_half_size));

            std::cout << msg1.data() << "\n";
            std::cout << msg2.data() << "\n";

            sock.send(msg1);
            msock::sleep(timeout);
            auto first_half = sock.recv();

            msock::sleep(timeout);

            sock.send(msg2);
            msock::sleep(timeout);
            auto second_half = sock.recv();

            first_half = get_msg_piece(first_half);
            if (!second_half.empty())
                second_half = get_msg_piece(second_half);

            if (!second_half.empty())
                first_half.insert(first_half.end(), second_half.begin(), second_half.end());

            piece = std::move(first_half);
        }
        else
        {
            // if we don't need second half
            auto msg = create_msg(REQUEST, create_payload_request(piece_num, 0, first_half_size));

            sock.send(msg);
            msock::sleep(timeout);
            auto first_half = sock.recv();

            piece = get_msg_piece(first_half);
        }

        sha_headonly::SHA1 sha;
        sha.update(std::string(reinterpret_cast<const char *>(piece.data())));
        hash = sha.final();

        std::cout << "hash: " << piece_hash << "\n";
        std::cout << "piece: " << hash << "\n";

        timeout *= 2;

    } while (piece_hash != hash && max_attempts > attempt++);


    if (piece_hash != hash)
    {
        disconect();
        return false;
    }

    file.write(piece.data(), piece_length * piece_num, first_half_size + second_half_size);

    return true;
}

