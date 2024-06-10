#pragma once

#include <string>

#include "bencoder.h"

class TFileParser final 
{
public:
    std::string get_url ();
    std::string get_size ();
    void parse_file(const std::string& file);

    static std::string open_file (const std::string& file_name);

private:
    std::vector<std::string> file_data;
};