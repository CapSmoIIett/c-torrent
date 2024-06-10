
#include <gtest/gtest.h>  

#include"../c-torrent/file_parser.h"

#define TEST_FILE_1 "Armageddon.torrent"
#define TEST_FILE_2 "sample.torrent"


TEST(FILE_PARSE_TEST, ReadFileTest) 
{
    //EXPECT_EQ(l.size(), r.size());
}

TEST(FILE_PARSE_TEST, ParseFile) 
{
    TFileParser parser;
    parser.parse_file(TFileParser::open_file(TEST_FILE_2));

    EXPECT_EQ("http://bittorrent-test-tracker.codecrafters.io/announce", parser.get_url());
    EXPECT_EQ("92063", parser.get_size());

}