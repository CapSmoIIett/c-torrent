#pragma once

#include "metainfo.h"
#include <vector>
#include <string>



#include "peer.h"

class BitTorrent
{
public:
    BitTorrent() = delete;
    BitTorrent(MetaInfo info) :
        minfo(info)
    { }

    std::vector<Peer> request_get_peers();
    
    std::string download_piece();

    void download (std::string file_name);

    MetaInfo minfo;

private:
    std::vector<Peer> peers = { };

};
