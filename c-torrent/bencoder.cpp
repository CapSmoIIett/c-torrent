#include "bencoder.h"


/*
static std::string Bencoder::encode()
{
    return "";
}
*/

std::string Bencoder::encode(std::vector<std::string> elements)
{
    if (elements.empty())
        return "";

    std::string result;

    if (1 < elements.size())
        result += "d";

    for (auto element : elements)
    {
        if (std::isdigit(element[0]) ||
            (2 <= element.size() && 
            std::isdigit(element[1]) &&
            '-' == element[0]))
        {
            result += "i" + element + "e";
        }
        else
        {
            result += std::to_string(element.size()) + ":" + element;
        }
    }

    if (1 < elements.size())
        result += "e";

    return result; 
}

std::vector<std::string> Bencoder::decode(std::string input)
{
    if (3 > input.size())
        return {""};

    if (std::isdigit(input[0]))    
    {
        return { Bencoder::decode_string(input, 0) };
    }
    else if ('i' == input[0])
    {
        return { Bencoder::decode_number(input, 0) };
    }
    else if ('d' == input[0])
    {
        return decode_array(input, 0);
    }

    return {};
}

std::string Bencoder::decode_string(const std::string& str, size_t it, size_t* end)
{
    size_t colon_index = str.find(':', it);

    if (colon_index == std::string::npos) 
    {
        if (nullptr != end)
            (*end)++;
        return "";
    }

    int64_t number = std::atoll(str.substr(it, colon_index).c_str());

    if (nullptr != end)
        *end = colon_index + 1 + number;

    return str.substr(colon_index + 1, number);
}

std::string Bencoder::decode_number(const std::string& str, size_t it, size_t* end)
{
    size_t begin_index = str.find('i', it);
    size_t end_index = str.find('e', it);

    if (begin_index == std::string::npos ||
        end_index == std::string::npos ||
        end_index - begin_index - 1 <= 0) 
    {
        if (nullptr != end)
            (*end)++;
        return "";
    }
    
    if (nullptr != end)
        *end = end_index;

    return str.substr(begin_index + 1, end_index - begin_index - 1);
}

std::vector<std::string> Bencoder::decode_array(const std::string& str, size_t it)
{
    std::vector<std::string> answer;

    while (it < str.size())
    {
        if (std::isdigit(str[it]))    
        {
            answer.push_back(decode_string(str, it, &it));
            continue;
        }

        if ('i' == str[it])
        {
            answer.push_back(Bencoder::decode_number(str, it, &it));
            continue;
        }

        ++it;
    }

    return answer;
}