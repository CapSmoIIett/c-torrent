#pragma once

#include <string>
#include <vector>

struct MetaInfo
{
public:
    struct Info
    {
        // fields with _* common for both mode
        std::string _piece_length = "";
        std::string _pieces = "";    // string consisting of the concatenation of all 20-byte SHA1 hash value
        std::string _private = "";
        std::string _name = "";  // name of file or dir
        
        std::string length = "";
        std::string md5sum = "";


        struct File
        {
            std::string length = "";
            std::string md5sum = "";
            std::string path = "";
        };

        std::vector<File> files = {};

    } info = {};

    std::string announce = "";
    std::vector<std::string> announce_list = {};
    std::string creation_date = "";
    std::string comment = "";
    std::string created_by = "";
    std::string encoding = "";
};
