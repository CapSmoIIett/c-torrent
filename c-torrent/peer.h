#pragma once 

#include <vector>

struct Peer
{
    Peer();
    Peer(std::vector<int> ip_port);

    int ip[4] = {0};
    int port;
};