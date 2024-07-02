#pragma once

#include "metainfo.h"
#include <vector>
#include <string>

std::vector<std::vector<long long int>> request_get_nodes(const MetaInfo minfo);

std::string request_get_peer_id(const MetaInfo minfo, std::string peer_ip, std::string peer_port);