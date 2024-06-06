
#include <gtest/gtest.h>  

template<class T>
void COMPARE_ARRAY(std::vector<T> l, std::vector<T> r)
{
    EXPECT_EQ(l.size(), r.size());

    for (int i = 0; i < l.size(); ++i) {
        EXPECT_EQ(l[i], r[i]) << "Arrays is different at index " 
            << i << ". "  << "l[i]: " << l[i] << ", r[i]: " << r[i]; 
    }
}