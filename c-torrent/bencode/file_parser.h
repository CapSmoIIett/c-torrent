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
    std::string get_created_by() const;
    std::string get_pieces() const;
    std::vector<std::string> get_announce_list() const;
    std::string get_field(const std::string& field) const;

    bool isSingleFile() const;

    void parse_file(const std::string& file);

    static std::string open_file (const std::string& file_name);

    std::vector<std::string> file_data;

private:

};