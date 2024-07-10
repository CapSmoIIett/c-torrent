
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
        Peer({ 165, 232, 33, 77, 51467 }),
        Peer({ 178, 62, 85, 20, 51489 }),
        Peer({ 178, 62, 82, 89, 51448 })},
        torrent.request_get_peers()
    );

}


#include <iostream>
TEST(REQUESR_TEST, TestRequest2) 
{
    /*
    
    system("pause");
    TFileParser parser;

    parser.parse_file(TFileParser::open_file(TEST_FILE_1));

    auto arr = request_get_nodes(get_meta_info(parser));


    for (auto it : arr )
    {
        for (auto i : it )
            std::cout << i << ".";
        std::cout << "\n";
    }
    */
}


TEST(REQUESR_TEST, PeerIdTest) 
{
    TFileParser parser;

    parser.parse_file(TFileParser::open_file(TEST_FILE_2));

    BitTorrent torrent(get_meta_info(parser));
    
    auto arr = torrent.request_get_peers();

    COMPARE_ARRAY_EQ({
        Peer({ 165, 232, 33, 77, 51467 }),
        Peer({ 178, 62, 85, 20, 51489 }),
        Peer({ 178, 62, 82, 89, 51448 })},
        arr//torrent.request_get_peers()
    );

    auto peer = arr[0];

    peer.connect();
    
    EXPECT_EQ(
        "2d524e302e302e302d5af5c2cf488815c4a2fa7f",
        peer.request_get_peer_id(torrent.minfo)
    );
}

TEST(REQUESR_TEST, DownloadPieceTest) 
{
    system("pause");
    TFileParser parser;

    parser.parse_file(TFileParser::open_file(TEST_FILE_2));

    BitTorrent torrent(get_meta_info(parser));

    torrent.download_piece();
}
