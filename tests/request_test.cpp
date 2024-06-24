
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

    //for (int i = 0; i < 10; ++i)
        request_get_nodes(get_meta_info(parser));
}

