
#include <gtest/gtest.h>  

#include "helper.h"
#include"../c-torrent/bencode/file_parser.h"
#include"../c-torrent/hash/hash.h"

#define TEST_FILE_1 "Armageddon.torrent"
#define TEST_FILE_2 "sample.torrent"


const std::vector<std::string> TEST_FILE_1_ANNOUNCE_LIST = {
    "udp://opentor.net:6969",
    "udp://tracker.ccc.de:80/announce",
    "http://retracker.local/announce",
    "udp://tracker.publicbt.com:80",
    "udp://tracker.istole.it:80",
    "udp://open.demonii.com:1337",
    "http://tracker.grepler.com:6969/announce",
    "udp://tracker.dler.com:6969/announce",
    "http://h4.trakx.nibba.trade:80/announce",
    "udp://open.stealth.si:80/announce",
    "udp://tracker.bitsearch.to:1337/announce",
    "udp://exodus.desync.com:6969/announce"
};


TEST(FILE_PARSE_TEST, ReadFileTest) 
{
    //EXPECT_EQ(l.size(), r.size());
}

TEST(FILE_PARSE_TEST, ParseFile_1) 
{
    TFileParser parser;
    parser.parse_file(TFileParser::open_file(TEST_FILE_1));

    COMPARE_ARRAY_EQ_SEQ(TEST_FILE_1_ANNOUNCE_LIST, parser.get_announce_list());

    EXPECT_EQ("udp://opentor.net:6969", parser.get_url());
    EXPECT_EQ("12403701089", parser.get_size());
    EXPECT_EQ("Armageddon.1998.BDRip.1080p.mkv", parser.get_name());
    EXPECT_EQ("8388608", parser.get_plength());
    EXPECT_EQ("1427552970", parser.get_creation_date());
    EXPECT_EQ("UTF-8", parser.get_encoding());
    EXPECT_EQ("http://rutor.is/torrent/421987", parser.get_comment());
    EXPECT_EQ("uTorrent/3200", parser.get_created_by());
}

TEST(FILE_PARSE_TEST, ParseFile_2) 
{
    TFileParser parser;
    parser.parse_file(TFileParser::open_file(TEST_FILE_2));

    EXPECT_EQ("http://bittorrent-test-tracker.codecrafters.io/announce", parser.get_url());
    EXPECT_EQ("92063", parser.get_size());
    EXPECT_EQ("32768", parser.get_plength());
}


TEST(CALCULATE_INFO_HASH, CalculateInfoHash)
{
    TFileParser parser;
    parser.parse_file(TFileParser::open_file(TEST_FILE_2));

    MetaInfo minfo = get_meta_info(parser);

    EXPECT_EQ("d69f91e6b2ae4c542468d1073a71d4ea13879a7f", calculate_info_hash(minfo));
}


TEST(GET_PIECES_TEST, GetPiecesTest)
{

    TFileParser parser;
    parser.parse_file(TFileParser::open_file(TEST_FILE_2));

    MetaInfo minfo = get_meta_info(parser);

    COMPARE_ARRAY_EQ_SEQ({
        "e876f67a2a8886e8f36b136726c30fa29703022d",
        "6e2275e604a0766656736e81ff10b55204ad8d35",
        "f00d937a0213df1982bc8d097227ad9e909acc17"},
        get_pieces(minfo.info._pieces)
    );
}
