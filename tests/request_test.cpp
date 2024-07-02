
#include <gtest/gtest.h>  


#include "helper.h"

#include "../c-torrent/metainfo.h"
#include "../c-torrent/file_parser.h"
#include "../c-torrent/hash.h"
#include "../c-torrent/request.h"

#define TEST_FILE_1 "Armageddon.torrent"
#define TEST_FILE_2 "sample.torrent"

TEST(REQUESR_TEST, TestRequest) 
{
    TFileParser parser;
    parser.parse_file(TFileParser::open_file(TEST_FILE_2));

    COMPARE_ARRAY_EQ({
        { 165, 232, 33, 77, 51467 },
        { 178, 62, 85, 20, 51489 },
        { 178, 62, 82, 89, 51448 }},
        request_get_nodes(get_meta_info(parser))
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
    
    auto arr = request_get_nodes(get_meta_info(parser));
    
    auto ip = std::to_string(arr[0][0]) + "." + std::to_string(arr[0][1]) + "." +
        std::to_string(arr[0][2]) + "." + std::to_string(arr[0][3]);

    EXPECT_EQ(
        "2d524e302e302e302d5af5c2cf488815c4a2fa7f",
        request_get_peer_id(get_meta_info(parser), ip, std::to_string(arr[0][4])));

}
