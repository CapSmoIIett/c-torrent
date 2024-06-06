
#include <gtest/gtest.h>  

#include "../c-torrent/bencoder.h"

TEST(BENCODE_DECODE_TEST, TestDecodeString) 
{
  EXPECT_EQ({"hello"}, bencoder.decode{"5:hello"});
  EXPECT_TRUE(true);
}