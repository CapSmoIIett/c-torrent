
#include "peer.h"


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