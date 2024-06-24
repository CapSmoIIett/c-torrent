
#include "request.h"
//#include "HTTPRequest/include/HTTPRequest.hpp"
#include "httplib/httplib.h"

#include "hash.h" 


#include <iostream>

std::vector<std::string> request_get_nodes(const MetaInfo minfo)
{
/*
    http::Request request(minfo.announce);
    const std::string body = "foo=1&bar=baz";

    const auto response = request.send("GET", body);

    std::cout << std::string{response.body.begin(), response.body.end()} << '\n'; 
*/

    std::string peer_id = "00112233445566778899";
    size_t port = 6881;
    size_t uploaded = 0;
    size_t downloaded = 0;
    size_t compact = 1;
    httplib::Client cli(minfo.announce);

    httplib::Headers headers = {
        { "info_hash", calculate_info_hash(minfo)},
        { "peer_id", peer_id},
        { "port", std::to_string(port)},
        { "uploaded", std::to_string(uploaded)},
        { "downloaded", std::to_string(downloaded)},
        { "left", minfo.info.length},
        { "compact", std::to_string(compact)},
    };

    auto res = cli.Get("", headers);

    std::cout << res << "\n";

    return {};
}