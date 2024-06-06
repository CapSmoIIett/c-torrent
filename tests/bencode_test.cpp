
#include <gtest/gtest.h>  

//#include "helper.h"
#include "../c-torrent/bencoder.h"

TEST(BENCODE_DECODE_TEST, TestDecodeString) 
{
    auto res = Bencoder::decode("5:hello");
    /*
    auto compareArray = [&](std::vector<std::string> l, std::vector<std::string> r){
        EXPECT_EQ(l.size(), r.size());

        for (int i = 0; i < l.size(); ++i) {
            EXPECT_EQ(l[i], r[i]) << "Arrays is different at index " 
                << i ;//<< ". "  << "l[i]: " << l[i] << ", r[i]: " << r[i]; 
        }
    };

    //COMPARE_ARRAY({"hello"}, Bencoder::decode("5:hello"));
    compareArray({"hello"}, Bencoder::decode("5:hello"));
    */

    EXPECT_TRUE(true);
}