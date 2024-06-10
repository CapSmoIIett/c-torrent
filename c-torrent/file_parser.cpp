#include "file_parser.h"

#include <fstream>
#include <sstream>
#include <streambuf>
#include <algorithm>

std::string TFileParser::get_url()
{
    auto it = std::find(file_data.begin(), file_data.end(), "announce");

    if (file_data.end() == it)
        return "";

    return *(++it);
}

std::string TFileParser::get_size()
{
    auto it = std::find(file_data.begin(), file_data.end(), "length");

    if (file_data.end() == it)
        return "";

    return *(++it);
}

void TFileParser::parse_file(const std::string& file)
{
    this->file_data = Bencoder::decode(file);
}

std::string TFileParser::open_file (const std::string& file_name)
{
    auto fp = std::fopen(file_name.c_str(), "rb");

    std::fseek(fp, 0u, SEEK_END);
    auto size = std::ftell(fp);
    std::fseek(fp, 0u, SEEK_SET);
    std::string s(size, '\0');
    std::fread(&s[0], 1u, size, fp);

    std::fclose(fp); 

    return s;
}