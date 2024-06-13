
#include <gtest/gtest.h>  

#include"../c-torrent/file_parser.h"

#define TEST_FILE_1 "Armageddon.torrent"
#define TEST_FILE_2 "sample.torrent"


TEST(FILE_PARSE_TEST, ReadFileTest) 
{
    //EXPECT_EQ(l.size(), r.size());
}

TEST(FILE_PARSE_TEST, ParseFile_1) 
{
    TFileParser parser;
    parser.parse_file(TFileParser::open_file(TEST_FILE_1));

    EXPECT_EQ("udp://opentor.net:6969", parser.get_url());
    EXPECT_EQ("12403701089", parser.get_size());
    EXPECT_EQ("Armageddon.1998.BDRip.1080p.mkv", parser.get_name());
    EXPECT_EQ("8388608", parser.get_plength());
    EXPECT_EQ("1427552970", parser.get_creation_date());
    EXPECT_EQ("UTF-8", parser.get_encoding());
}

TEST(FILE_PARSE_TEST, ParseFile_2) 
{
    TFileParser parser;
    parser.parse_file(TFileParser::open_file(TEST_FILE_2));

    EXPECT_EQ("http://bittorrent-test-tracker.codecrafters.io/announce", parser.get_url());
    EXPECT_EQ("92063", parser.get_size());
    EXPECT_EQ("32768", parser.get_plength());
}