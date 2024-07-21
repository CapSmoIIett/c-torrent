#pragma once

#include <gtest/gtest.h>  
#include <fstream>

template<class T>
void COMPARE_ARRAY_EQ_SEQ(std::vector<T> l, std::vector<T> r)
{
    EXPECT_EQ(l.size(), r.size());

    if (l.size() != r.size())
        return;

    for (int i = 0; i < l.size(); ++i) {
        EXPECT_EQ(l[i], r[i]) << "Arrays is different at index " 
            << i << ". "  << "l[i]: " << l[i] << ", r[i]: " << r[i]; 
    }
}

template<class T>
void COMPARE_ARRAY_EQ(std::vector<T> l, std::vector<T> r)
{
    EXPECT_EQ(l.size(), r.size());

    if (l.size() != r.size())
        return;

    for (int i = 0; i < l.size(); ++i) {
        auto it = std::find(r.begin(), r.end(), l[i]);

        EXPECT_TRUE(r.end() != it) << "Arrays is different. " 
            << "right array does not have: " << l[i];

        if (r.end() == it)
            return;

    }
}

template<class T>
void COMPARE_ARRAY_NE(std::vector<T> l, std::vector<T> r)
{
    if (l.size() != r.size())
    {
        EXPECT_NE(l.size(), r.size());
        return;
    }

    for (int i = 0; i < l.size(); ++i) {
        EXPECT_NE(l[i], r[i]) << "Arrays is same at index " 
            << i << ". "  << "l[i]: " << l[i] << ", r[i]: " << r[i]; 
    }
}


template<class T>
void COMPARE_ARRAY_EQ_SEQ(std::vector<std::vector<T>> l, std::vector<std::vector<T>> r)
{
    EXPECT_EQ(l.size(), r.size());

    if (l.size() != r.size())
        return;
    
    for (int i = 0; i < l.size(); ++i)
    {
        EXPECT_EQ(l[i].size(), r[i].size());

        if (l[i].size() != r[i].size())
            return;

        for (int j = 0; j < l[i].size(); ++j) 
        {
            EXPECT_EQ(l[i][j], r[i][j]) << "Arrays is different at index " 
                << i << " " << j << ". "  << "l[i][j]: " << l[i][j] << ", r[i]: " << r[i][j]; 
        }
    }
}

template<class T>
void COMPARE_ARRAY_NE(std::vector<std::vector<T>> l, std::vector<std::vector<T>> r)
{
    if (l.size() != r.size())
    {
        EXPECT_NE(l.size(), r.size());
        return;
    }
    
    for (int i = 0; i < l.size(); ++i)
    {
        if (l[i].size() != r[i].size())
        {
            EXPECT_NE(l[i].size(), r[i].size());
            return;
        }

        for (int j = 0; j < l[i].size(); ++j) 
        {
            EXPECT_NE(l[i][j], r[i][j]) << "Arrays is same at index " 
                << i << " " << j << ". "  << "l[i][j]: " << l[i][j] << ", r[i]: " << r[i][j]; 
        }
    }
}

inline void COMPARE_FILES_EQ(const std::string& name_left, const std::string& name_right)
{
    std::ifstream file_left(name_left, std::ios::binary);
    std::ifstream file_right(name_right, std::ios::binary);

    if (!file_left.is_open())
        EXPECT_TRUE(file_left.is_open());

    if (!file_right.is_open())
        EXPECT_TRUE(file_right.is_open());

    std::istreambuf_iterator<char> it_left(file_left);
    std::istreambuf_iterator<char> it_right(file_right);
    std::istreambuf_iterator<char> end;

    size_t symbol = 0;
    while(it_left != end && it_right != end) 
    { 
        if(*it_left != *it_right) 
        {
            EXPECT_EQ(*it_left, *it_right) 
                << "Characters are different. Left: " << *it_left << " Right: " << *it_right << " i: " << symbol;
            return;
        }

        ++symbol;
        ++it_left;
        ++it_right;
    }

    EXPECT_EQ(it_left == end, it_right == end);

}