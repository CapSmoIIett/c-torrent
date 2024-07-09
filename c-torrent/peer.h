#pragma once 

#include <vector>
#include <iostream>

#include "socket.h"
#include "metainfo.h"

struct Peer
{
    Peer() = default;
    Peer(std::vector<int> ip_port);
    Peer(const Peer&) = default;

    void connect();
    std::string request_get_peer_id(MetaInfo minfo);
    bool operator==(const Peer& peer) const;
    friend std::ostream& operator<<( std::ostream& os, const Peer& mc);

    int ip[4] = {0};
    int port = 0;

    msock::Socket sock;
};