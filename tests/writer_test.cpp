
#include <gtest/gtest.h>  

#include <algorithm>
#include <random>

#include "helper.h"
#include "../c-torrent/async_writer.h"

#define TEST_FILE_WRITE "test.txt"

const std::vector<std::string> test_vec =
{
    "The path of the righteous man is beset on all sides by",
    " the iniquities of the selfish and the tyranny of evil",
    " men. Blessed is He who in the name of charity and goo",
    "d will shepherds the weak through the valley of darkne",
    "ss, for He is truly his brother's keeper and the finde",
    "r of lost children. [begins pacing about the room] And",
    " I will strike down upon thee with great vengeance and" ,
    " furious anger those who attempt to poison and destroy",
    " My brothers. And you will know My name is the Lord...",
    " [pulls out his gun and aims at Brett] ...when I lay  ",
    "My vengeance upon thee."
};

const size_t str_size = test_vec[0].size();

const std::string test_str = 
    "The path of the righteous man is beset on all sides by"
    " the iniquities of the selfish and the tyranny of evil"
    " men. Blessed is He who in the name of charity and goo"
    "d will shepherds the weak through the valley of darkne"
    "ss, for He is truly his brother's keeper and the finde"
    "r of lost children. [begins pacing about the room] And"
    " I will strike down upon thee with great vengeance and" 
    " furious anger those who attempt to poison and destroy"
    " My brothers. And you will know My name is the Lord..."
    " [pulls out his gun and aims at Brett] ...when I lay  "
    "My vengeance upon thee.";

TEST(WRITER_TEST, Write) 
{
    //std::srand (unsigned(std::time(0)));

    // test fiew times
    for (int i = 0; i < 5; ++i)
    {
        AsyncWriter file(TEST_FILE_WRITE);

        std::vector<int> nums = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        //auto rng = std::default_random_engine {};
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(nums.begin(), nums.end(), g);

        for (auto n : nums)
        {
            file.write(test_vec[n], str_size * n);

        }
        
        file.close();
        
        COMPARE_FILE_STR_EQ(TEST_FILE_WRITE, test_str);
    }

}
