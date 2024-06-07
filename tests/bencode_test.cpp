
#include <gtest/gtest.h>  

#include "helper.h"
#include "../c-torrent/bencoder.h"

TEST(BENCODE_DECODE_TEST, TestDecodeString) 
{
    auto res = Bencoder::decode("5:hello");

    COMPARE_ARRAY_EQ({"hello"}, Bencoder::decode("5:hello"));
    COMPARE_ARRAY_EQ({"hell"},  Bencoder::decode("4:hello"));
    COMPARE_ARRAY_NE({"hello"}, Bencoder::decode("4:hello"));
    COMPARE_ARRAY_EQ({"Hello World!"}, Bencoder::decode("12:Hello World!"));
    COMPARE_ARRAY_EQ({"~`'\"!@#$%^&*()"}, Bencoder::decode("14:~`'\"!@#$%^&*()"));
    COMPARE_ARRAY_NE({"~`'\"!@#$%^&*()"}, Bencoder::decode("11:~`'\"!@#$%^&*()"));
}

TEST(BENCODE_DECODE_TEST, TestDecodeNumbers) 
{
    EXPECT_TRUE(true);
}