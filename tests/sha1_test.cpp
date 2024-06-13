
#include <gtest/gtest.h>  

#include"../c-torrent/sha1.h"

TEST(SHA1_TESTS, sha1_Test) 
{
    SHA1 checksum;

    checksum.update("abc");
    EXPECT_EQ("a9993e364706816aba3e25717850c26c9cd0d89d", checksum.final());

    checksum.update("cba");
    EXPECT_EQ("d9f0509fb7e8bd7d4c4b627dfec70c0c0e01fb34", checksum.final());

    checksum.update("qwerty");
    EXPECT_EQ("b1b3773a05c0ed0176787a4f1574ff0075f7521e", checksum.final());
}


