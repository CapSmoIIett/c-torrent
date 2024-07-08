#pragma once

#include "metainfo.h"
#include <vector>
#include <string>

//#include "socket.h"
#include "peer.h"

class BitTorrent
{
public:
    BitTorrent() = delete;
    BitTorrent(MetaInfo info) :
        minfo(info)
    { }

    std::vector<Peer> request_get_peers();

    MetaInfo minfo;

private:
    std::vector<Peer> peers = { };

};