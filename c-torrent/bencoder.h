#pragma once

#include <vector>
#include <string>

class Bencoder
{
public:
    static std::string encode();
    static std::vector<std::string> dencode(std::string input);
};