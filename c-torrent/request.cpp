
#include "request.h"
//#include "HTTPRequest/include/HTTPRequest.hpp"
#include "httplib/httplib.h"

#include "hash.h" 


#include <iostream>

std::tuple<std::string, std::string> split_domain_and_endpoint(const std::string& tracker_url) {
    auto last_forward_slash_index = tracker_url.find_last_of('/');
    auto domain = tracker_url.substr(0, last_forward_slash_index);
    auto endpoint = tracker_url.substr(last_forward_slash_index, tracker_url.size() - last_forward_slash_index);
    return std::make_tuple(domain, endpoint);
}

std::vector<std::string> request_get_nodes(const MetaInfo minfo)
{
/*
    http::Request request(minfo.announce);
    const std::string body = "foo=1&bar=baz";

    const auto response = request.send("GET", body);

    std::cout << std::string{response.body.begin(), response.body.end()} << '\n'; 
*/

    size_t port = 6881;
    size_t uploaded = 0;
    size_t downloaded = 0;
    size_t compact = 1;

    httplib::Params params{
        {"peer_id", "00112233445566778899"},
        { "port", std::to_string(port)},
        {"uploaded", "0"},
        {"downloaded", "0"},
        { "left", minfo.info.length},
        {"compact", "1"}
    };

    httplib::Headers headers{};


    auto domain_and_endpoint = split_domain_and_endpoint(minfo.announce);

    //httplib::Client cli(minfo.announce);
    httplib::Client cli(std::get<0>(domain_and_endpoint));
    //auto res = cli.Get("", headers);
    auto res = cli.Get(
        std::get<1>(domain_and_endpoint) + "?info_hash=d69f91e6b2ae4c542468d1073a71d4ea13879a7f", //calculate_info_hash(minfo),
        params,
        headers
    );

    std::cout << res->body << "\n";

    return {};
}