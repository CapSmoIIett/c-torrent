#include "file_parser.h"

#include <fstream>
#include <sstream>
#include <streambuf>
#include <algorithm>


std::string TFileParser::get_field(const std::string& field) const 
{
    auto it = std::find(file_data.begin(), file_data.end(), field);

    if (file_data.end() == it)
        return "";

    if (file_data.end() == ++it)
        return "";

    return *it;
}

std::string TFileParser::get_size() const
{
    return get_field("length");
}

std::string TFileParser::get_url() const
{
    return get_field("announce");
}

std::string TFileParser::get_plength() const
{
    return get_field("piece length");
}

std::string TFileParser::get_creation_date() const
{
    return get_field("creation date");
}

std::string TFileParser::get_comment() const
{
    return get_field("comment");
}

std::string TFileParser::get_name() const
{
    return get_field("name");
}

std::string TFileParser::get_encoding() const
{
    return get_field("encoding");
}

std::string TFileParser::get_created_by() const
{
    return get_field("created by");
}

std::vector<std::string> TFileParser::get_announce_list() const
{
    auto it = std::find(file_data.begin(), file_data.end(), "announce-list");

    if (file_data.end() == it)
        return {};

    std::vector<std::string> result;

    while (file_data.end() != ++it &&
           std::string::npos != ((it)->find("://")))
    {
        result.push_back(*it);
    }

    return result;
}

void TFileParser::parse_file(const std::string& file) 
{
    this->file_data = Bencoder::decode(file);
}

std::string TFileParser::open_file(const std::string& file_name)
{
    /*
    std::ifstream fp(file_name, std::ios::in | std::ios::binary | std::ios::ate);
    const auto size = fp.tellg();
    std::string s;
    s.resize(size);
    fp.seekg(0u);
    fp.read(&s[0], size);
    return s;
    */
    auto fp = std::fopen(file_name.c_str(), "rb");

    std::fseek(fp, 0u, SEEK_END);
    auto size = std::ftell(fp);
    std::fseek(fp, 0u, SEEK_SET);
    std::string s(size, '\0');
    std::fread(&s[0], 1u, size, fp);

    std::fclose(fp); 

    return s;
}