
#include <gtest/gtest.h>  


#include "helper.h"

#include "../c-torrent/metainfo.h"
#include "../c-torrent/file_parser.h"
#include "../c-torrent/hash.h"
#include "../c-torrent/torrent.h"

#define TEST_FILE_1 "Armageddon.torrent"
#define TEST_FILE_2 "sample.torrent"

TEST(REQUESR_TEST, TestGetPeers) 
{
    TFileParser parser;
    parser.parse_file(TFileParser::open_file(TEST_FILE_2));

    BitTorrent torrent(get_meta_info(parser));

    COMPARE_ARRAY_EQ({
        Peer({ 165, 232, 33, 77, 51498 }),
        Peer({ 178, 62, 85, 20, 51489 }),
        Peer({ 178, 62, 82, 89, 51448 })},
        torrent.request_get_peers()
    );

}


#include <iostream>
TEST(REQUESR_TEST, TestGetPeers2) 
{
    TFileParser parser;
    parser.parse_file(TFileParser::open_file(TEST_FILE_1));

    BitTorrent torrent(get_meta_info(parser));

    auto arr = torrent.request_get_peers();
    //COMPARE_ARRAY_EQ({
    //    Peer({ 165, 232, 33, 77, 51498 }),
    //    Peer({ 178, 62, 85, 20, 51489 }),
    //    Peer({ 178, 62, 82, 89, 51448 })},
    //);

    for (auto it : arr )
    {
        std::cout 
        << it.ip[0] << "."
        << it.ip[1] << "."
        << it.ip[2] << "."
        << it.ip[3] << ":"
        << it.port << "\n";
    }
}


TEST(REQUESR_TEST, PeerIdTest) 
{
    TFileParser parser;

    parser.parse_file(TFileParser::open_file(TEST_FILE_2));

    BitTorrent torrent(get_meta_info(parser));
    
    auto arr = torrent.request_get_peers();

    COMPARE_ARRAY_EQ({
        Peer({ 165, 232, 33, 77, 51498 }),
        Peer({ 178, 62, 85, 20, 51489 }),
        Peer({ 178, 62, 82, 89, 51448 })},
        arr//torrent.request_get_peers()
    );

    auto peer = arr[1];

    peer.connect();

    std::vector<std::string> ids = {
    "2d524e302e302e302d5af5c2cf488815c4a2fa7f",
    "2d524e302e302e302d0df778d11e39f854451c24"
    };
    
    EXPECT_EQ(
        ids[1],
        peer.request_get_peer_id(torrent.minfo)
    );
}

TEST(REQUESR_TEST, DownloadPieceTest) 
{
    TFileParser parser;

    parser.parse_file(TFileParser::open_file(TEST_FILE_2));

    BitTorrent torrent(get_meta_info(parser));

    torrent.download("_" + torrent.minfo.info._name);

    COMPARE_FILES_EQ(torrent.minfo.info._name, "_" + torrent.minfo.info._name);
}

