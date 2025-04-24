
#include <gtest/gtest.h>  
#include <gmock/gmock.h>


#include "helper.h"

#include "../c-torrent/metainfo/metainfo.h"
#include "../c-torrent/bencode/file_parser.h"
#include "../c-torrent/hash/hash.h"
#include "../c-torrent/torrent/torrent.h"

#define TEST_FILE_1 "Armageddon.torrent"
#define TEST_FILE_2 "sample.torrent"
#define TEST_FILE_UBUNTU "ubuntu-24.04-desktop-amd64.iso.torrent"

static const std::vector<Peer> PEER_ARRAY_1 = {
        Peer({ 161, 35, 46, 221, 51414}),
        Peer({ 159, 65, 84, 183, 51444}),
        Peer({ 167, 172, 57, 188, 51413})
    };

TEST(REQUEST_TEST, TestGetPeers) 
{
    TFileParser parser;
    parser.parse_file(TFileParser::open_file(TEST_FILE_2));

    BitTorrent torrent(get_meta_info(parser));

    COMPARE_ARRAY_EQ(
        PEER_ARRAY_1,
        torrent.request_get_peers()
    );

}


/*
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
TEST(REQUEST_TEST, TestGetPeersUbuntu) 
{
    TFileParser parser;
    parser.parse_file(TFileParser::open_file(TEST_FILE_UBUNTU));

    BitTorrent torrent(get_meta_info(parser));

    COMPARE_ARRAY_EQ({Peer({ 185, 125, 190, 59, 6894})},
        torrent.request_get_peers()
    );
}
#endif
*/


TEST(REQUEST_TEST, PeerIdTest) 
{
    TFileParser parser;

    parser.parse_file(TFileParser::open_file(TEST_FILE_2));

    BitTorrent torrent(get_meta_info(parser));
    
    auto arr = torrent.request_get_peers();

    for (auto peer : arr)
        std::cout << peer << "\n";

    COMPARE_ARRAY_EQ(
        PEER_ARRAY_1,
        arr//torrent.request_get_peers()
    );

    auto peer = arr[1];

    peer.connect();

    static const std::vector<std::string> ids = {
    "2d524e302e302e302d5af5c2cf488815c4a2fa7f",
    "2d524e302e302e302d0df778d11e39f854451c24",
    "2d524e302e302e302d71436ef031d3d90fc6cb18",
    "2d524e302e302e302db4166a8c5ee1c01d1c6263",
    "2d524e302e302e302dc3381e92ac5122cdeb08db"
    };

    auto peer_id = peer.request_get_peer_id(torrent.minfo);

    std::cout << peer_id << "\n";

    ASSERT_THAT(peer_id, ::testing::AnyOfArray(ids));
}

TEST(REQUEST_TEST, DownloadPieceTest) 
{
    TFileParser parser;
    const char temp_file_name[] = "__temp.txt";

    parser.parse_file(TFileParser::open_file(TEST_FILE_2));

    MetaInfo minfo = get_meta_info(parser);

    BitTorrent torrent(minfo);

    AsyncWriter file(temp_file_name);

    auto peers = torrent.request_get_peers();
    if (peers.empty())
        return;

    auto peer = &peers[1];
    peer->connect();

    auto peer_id = peer->request_get_peer_id(minfo);

    // read bitfield
    //peer->sock.recv();

    EXPECT_FALSE(peer_id.empty());

    if (peer_id.empty())
        return;

    peer->send_interested();

    auto pieces = get_pieces(minfo.info._pieces);

    peer->download_piece(file, minfo, 0);

    auto download = READ_FILE(temp_file_name);
    auto reference = READ_FILE("sample.txt", 0, std::stoi(minfo.info._piece_length));

    EXPECT_EQ(download, reference);

    remove(temp_file_name);
}

TEST(REQUEST_TEST, DownloadTest) 
{
    TFileParser parser;

    parser.parse_file(TFileParser::open_file(TEST_FILE_2));

    BitTorrent torrent(get_meta_info(parser));

    torrent.download("_" + torrent.minfo.info._name);

    COMPARE_FILES_EQ(torrent.minfo.info._name, "_" + torrent.minfo.info._name);
}

