#pragma once

#include <string>
#include <vector>
#include "file_parser.h"

struct MetaInfo
{
    MetaInfo (const TFileParser& parser);

    struct Info
    {
        // fields with _* common for both mode
        std::string _piece_length;
        std::vector<std::string> _pieces;
        std::string _private;
        std::string _name;  // name of file or dir
        
        std::string length;
        std::string md5sum;


        struct File
        {
            std::string length;
            std::string md5sum;
            std::string path;
        };

        std::vector<File> files;

    } info;

    std::string announce;
    std::vector<std::string> announce_list;
    std::string creation_date;
    std::string comment;
    std::string created_by;
    std::string encoding;


};