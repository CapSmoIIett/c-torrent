#include "bencoder.h"


/*
static std::string Bencoder::encode()
{
    return "";
}
*/

std::vector<std::string> Bencoder::decode(std::string input)
{
    if (3 > input.size())
        return {""};
    if (std::isdigit(input[0]))    
    {
        size_t colon_index = input.find(':');

        if (colon_index == std::string::npos) 
            return {""};


        int64_t number = std::atoll(input.substr(0, colon_index).c_str());
        auto str = input.substr(colon_index + 1, number);
        return {str};
    }

    return {};
}