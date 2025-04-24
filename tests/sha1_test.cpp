
#include <gtest/gtest.h>  

#include"../c-torrent/hash/sha1.h"

TEST(SHA1_TESTS, sha1_Test) 
{
    sha_headonly::SHA1 checksum;

    checksum.update("abc");
    EXPECT_EQ("a9993e364706816aba3e25717850c26c9cd0d89d", checksum.final());

    checksum.update("cba");
    EXPECT_EQ("d9f0509fb7e8bd7d4c4b627dfec70c0c0e01fb34", checksum.final());

    checksum.update("qwerty");
    EXPECT_EQ("b1b3773a05c0ed0176787a4f1574ff0075f7521e", checksum.final());
}

TEST(SHA1_TESTS, sha1_Test_2) 
{
    sha_headonly::SHA1_HASH hash;

    Sha1Calculate("abc", 3, &hash);
    EXPECT_EQ("a9993e364706816aba3e25717850c26c9cd0d89d", sha_headonly::HashToString(hash));

    Sha1Calculate("cba", 3, &hash);
    EXPECT_EQ("d9f0509fb7e8bd7d4c4b627dfec70c0c0e01fb34", sha_headonly::HashToString(hash));

    Sha1Calculate("qwerty", 6, &hash);
    EXPECT_EQ("b1b3773a05c0ed0176787a4f1574ff0075f7521e", sha_headonly::HashToString(hash));
}


