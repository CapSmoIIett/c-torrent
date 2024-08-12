#pragma once 

#include <vector>
#include <iostream>

#include "socket.h"
#include "metainfo.h"
#include "async_writer.h"


struct Peer
{
    Peer() = default;
    Peer(std::vector<int> ip_port);
    Peer(const Peer&) = default;

    void connect();
    void disconect();

    bool connected();

    void send_interested();
    
    std::string request_get_peer_id(MetaInfo minfo);

    bool download_piece(AsyncWriter& file, MetaInfo minfo, int piece_num);

    bool operator==(const Peer& peer) const;
    friend std::ostream& operator<<( std::ostream& os, const Peer& mc);

    int ip[4] = {0};
    int port = 0;

    msock::Socket sock;
};