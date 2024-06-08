#pragma once

#include <vector>
#include <string>

class Bencoder
{
public:
    //static std::string encode();
    static std::vector<std::string> decode(std::string input);

private:
    static std::string decode_string(const std::string& str, size_t it, size_t* end = nullptr);
    static std::string decode_number(const std::string& str, size_t it, size_t* end = nullptr);
    static std::vector<std::string> decode_array(const std::string& str, size_t it);
};