
#include <gtest/gtest.h>  

#include "helper.h"
#include "../c-torrent/bencode/bencoder.h"

TEST(BENCODE_DECODE_TEST, TestDecodeString) 
{
    COMPARE_ARRAY_EQ_SEQ({""}, Bencoder::decode("0:"));
    COMPARE_ARRAY_EQ_SEQ({""}, Bencoder::decode("1:"));
    COMPARE_ARRAY_EQ_SEQ({"hello"}, Bencoder::decode("5:hello"));
    COMPARE_ARRAY_EQ_SEQ({"hell"},  Bencoder::decode("4:hello"));
    COMPARE_ARRAY_NE({"hello"}, Bencoder::decode("4:hello"));
    COMPARE_ARRAY_EQ_SEQ({"Hello World!"}, Bencoder::decode("12:Hello World!"));
    COMPARE_ARRAY_EQ_SEQ({"~`'\"!@#$%^&*()"}, Bencoder::decode("14:~`'\"!@#$%^&*()"));
    COMPARE_ARRAY_NE({"~`'\"!@#$%^&*()"}, Bencoder::decode("11:~`'\"!@#$%^&*()"));
}

TEST(BENCODE_DECODE_TEST, TestDecodeNumbers) 
{
    COMPARE_ARRAY_EQ_SEQ({"42"}, Bencoder::decode("i42e"));
    COMPARE_ARRAY_EQ_SEQ({"-42"}, Bencoder::decode("i-42e"));
    COMPARE_ARRAY_EQ_SEQ({"0"}, Bencoder::decode("i0e"));
    COMPARE_ARRAY_NE({"-1"}, Bencoder::decode("i1e"));
    COMPARE_ARRAY_NE({"2"}, Bencoder::decode("i-2e"));
    COMPARE_ARRAY_NE({"3"}, Bencoder::decode("i-3"));
    COMPARE_ARRAY_NE({"4"}, Bencoder::decode("-4e"));
    COMPARE_ARRAY_NE({"5"}, Bencoder::decode("-5"));
    COMPARE_ARRAY_EQ_SEQ({""}, Bencoder::decode("ie"));
}

TEST(BENCODE_DECODE_TEST, TestDecodeDictionaries) 
{
    COMPARE_ARRAY_EQ_SEQ({"hello","World","!","42"}, Bencoder::decode("d5:hello5:World1:!i42ee"));
    COMPARE_ARRAY_EQ_SEQ({"hello","World","!","-42"}, Bencoder::decode("d5:hello5:World1:!i-42ee"));
    COMPARE_ARRAY_EQ_SEQ({"1","-2","3","-4"}, Bencoder::decode("di1ei-2ei3ei-4ee"));
    COMPARE_ARRAY_EQ_SEQ({""}, Bencoder::decode("de"));
    COMPARE_ARRAY_EQ_SEQ({""}, Bencoder::decode("e"));
    COMPARE_ARRAY_EQ_SEQ({""}, Bencoder::decode("d"));

}

TEST(BENCODE_ENCODE_TEST, TestEncodeStrings) 
{
    using namespace std::string_literals;
    EXPECT_EQ("0:", Bencoder::encode({""s}));
    EXPECT_EQ("1: ", Bencoder::encode({" "s}));
    EXPECT_EQ("5:hello", Bencoder::encode({"hello"s}));
    EXPECT_EQ("12:Hello World!", Bencoder::encode({"Hello World!"s}));
    EXPECT_EQ("14:~`'\"!@#$%^&*()", Bencoder::encode({"~`'\"!@#$%^&*()"s}));
}

TEST(BENCODE_ENCODE_TEST, TestEncodeNumbers) 
{
    using namespace std::string_literals;
    EXPECT_EQ("i42e", Bencoder::encode({"42"s}));
    EXPECT_EQ("i-42e", Bencoder::encode({"-42"s}));
    EXPECT_EQ("i0e", Bencoder::encode({"0"s}));
}

TEST(BENCODE_ENCODE_TEST, TestEncodeDictionaries) 
{
    EXPECT_EQ("d5:hello5:World1:!i42ee", Bencoder::encode({"hello","World","!","42"}));
    EXPECT_EQ("d5:hello5:World1:!i-42ee", Bencoder::encode({"hello","World","!","-42"}));
    EXPECT_EQ("di1ei-2ei3ei-4ee", Bencoder::encode({"1","-2","3","-4"}));
}