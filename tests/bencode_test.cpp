
#include <gtest/gtest.h>  

#include "helper.h"
#include "../c-torrent/bencoder.h"

TEST(BENCODE_DECODE_TEST, TestDecodeString) 
{
    COMPARE_ARRAY_EQ({""}, Bencoder::decode("0:"));
    COMPARE_ARRAY_EQ({""}, Bencoder::decode("1:"));
    COMPARE_ARRAY_EQ({"hello"}, Bencoder::decode("5:hello"));
    COMPARE_ARRAY_EQ({"hell"},  Bencoder::decode("4:hello"));
    COMPARE_ARRAY_NE({"hello"}, Bencoder::decode("4:hello"));
    COMPARE_ARRAY_EQ({"Hello World!"}, Bencoder::decode("12:Hello World!"));
    COMPARE_ARRAY_EQ({"~`'\"!@#$%^&*()"}, Bencoder::decode("14:~`'\"!@#$%^&*()"));
    COMPARE_ARRAY_NE({"~`'\"!@#$%^&*()"}, Bencoder::decode("11:~`'\"!@#$%^&*()"));
}

TEST(BENCODE_DECODE_TEST, TestDecodeNumbers) 
{
    COMPARE_ARRAY_EQ({"42"}, Bencoder::decode("i42e"));
    COMPARE_ARRAY_EQ({"-42"}, Bencoder::decode("i-42e"));
    COMPARE_ARRAY_EQ({"0"}, Bencoder::decode("i0e"));
    COMPARE_ARRAY_NE({"-1"}, Bencoder::decode("i1e"));
    COMPARE_ARRAY_NE({"2"}, Bencoder::decode("i-2e"));
    COMPARE_ARRAY_NE({"3"}, Bencoder::decode("i-3"));
    COMPARE_ARRAY_NE({"4"}, Bencoder::decode("-4e"));
    COMPARE_ARRAY_NE({"5"}, Bencoder::decode("-5"));
    COMPARE_ARRAY_EQ({""}, Bencoder::decode("ie"));
}

TEST(BENCODE_DECODE_TEST, TestDecodeDictionaries) 
{
    COMPARE_ARRAY_EQ({{"hello","World","!","42"}}, Bencoder::decode("d5:hello5:World1:!i42ee"));
    COMPARE_ARRAY_EQ({{"hello","World","!","-42"}}, Bencoder::decode("d5:hello5:World1:!i-42ee"));
    COMPARE_ARRAY_EQ({{"1","-2","3","-4"}}, Bencoder::decode("di1ei-2ei3ei-4ee"));
    COMPARE_ARRAY_EQ({{""}}, Bencoder::decode("de"));
    COMPARE_ARRAY_EQ({{""}}, Bencoder::decode("e"));
    COMPARE_ARRAY_EQ({{""}}, Bencoder::decode("d"));

}