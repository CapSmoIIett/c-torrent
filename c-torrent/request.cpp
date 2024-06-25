
#include "request.h"
//#include "HTTPRequest/include/HTTPRequest.hpp"
#include "httplib/httplib.h"
#include "../c-torrent/bencoder.h"

#include "hash.h" 
#include <tuple>


#include <iostream>

std::tuple<std::string, std::string> split_domain_and_endpoint(const std::string& tracker_url) {
    auto last_forward_slash_index = tracker_url.find_last_of('/');
    auto domain = tracker_url.substr(0, last_forward_slash_index);
    auto endpoint = tracker_url.substr(last_forward_slash_index, tracker_url.size() - last_forward_slash_index);
    return std::make_tuple(domain, endpoint);
}

std::string encode_info_hash(const std::string& hash) {
    std::string encoded;
    for(auto i = 0; i < hash.size(); i += 2) {
        encoded += '%' + hash.substr(i, 2);
    }
    return encoded;
}

std::vector<std::vector<long long int>> decode_peers(std::string& encoded_peers) {
    std::vector<std::vector<long long int>> peers{};

    for(auto i = 0; i < encoded_peers.size(); i += 6) 
    {
        auto peer = encoded_peers.substr(i, 6);
        // First four bytes are the ip, the final two bytes are the port in big endian.
        peers.push_back(
            {
                static_cast<uint8_t>(peer[0]),
                static_cast<uint8_t>(peer[1]),
                static_cast<uint8_t>(peer[2]),
                static_cast<uint8_t>(peer[3]),
                static_cast<uint8_t>(peer[4]) << 8 | peer[5]
            });
    }
    
    return peers;
}

std::vector<std::string> request_get_nodes(const MetaInfo minfo)
{
/*
    http::Request request(minfo.announce);
    const std::string body = "foo=1&bar=baz";

    const auto response = request.send("GET", body);

    std::cout << std::string{response.body.begin(), response.body.end()} << '\n'; 

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

    //std::cout << std::get<0>(domain_and_endpoint) << "\n";
    //std::cout << std::get<1>(domain_and_endpoint) << "\n";

    //httplib::Client cli(minfo.announce);
    //auto res = cli.Get("", headers);

    httplib::Client cli(std::get<0>(domain_and_endpoint));
    auto res = cli.Get(
        std::get<1>(domain_and_endpoint) + "?info_hash=" + 
            encode_info_hash(calculate_info_hash(minfo)),
        params,
        headers
    );


    auto str = Bencoder::decode(res->body);

    
    auto it = std::find(str.begin(), str.end(), "peers");

    if (str.end() == it)
        return {};

    if (str.end() == ++it)
        return {};

    auto b = decode_peers(*it);


    for (auto c : b)
    {
        for (auto j : c)
            std::cout << j << ".";
        std::cout << "\n";
    }

    for (auto c : str)
    {
        //std::cout << c << "\n";
    }
/*
    httplib::Client cli(minfo.announce);

    auto res = cli.Get(
        "?info_hash=" + calculate_info_hash(minfo),
        params,
        headers
    );
    */

    //std::cout << res->body << "\n";

    return {};
}