#pragma once

#include <gtest/gtest.h>  

template<class T>
void COMPARE_ARRAY_EQ(std::vector<T> l, std::vector<T> r)
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
void COMPARE_ARRAY_EQ(std::vector<std::vector<T>> l, std::vector<std::vector<T>> r)
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