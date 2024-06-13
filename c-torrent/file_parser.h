#pragma once

#include <string>

#include "bencoder.h"

class TFileParser final 
{
public:
    std::string get_url() const;
    std::string get_size() const;
    std::string get_plength() const;
    std::string get_creation_date() const;
    std::string get_comment() const;
    std::string get_name() const;
    std::string get_encoding() const;
    std::string get_field(const std::string& field) const;
    void parse_file(const std::string& file);

    static std::string open_file (const std::string& file_name);

private:

    std::vector<std::string> file_data;
};