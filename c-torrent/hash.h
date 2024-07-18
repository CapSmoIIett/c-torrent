#pragma once

#include "metainfo.h"
#include "file_parser.h"
#include "bencoder.h"
#include "sha1.h"


inline MetaInfo get_meta_info(TFileParser parser)
{
    MetaInfo minfo;    
    minfo.announce = parser.get_url();
    minfo.announce_list = parser.get_announce_list();
    minfo.creation_date = parser.get_creation_date();
    minfo.comment = parser.get_comment();
    minfo.created_by = parser.get_created_by();
    minfo.encoding = parser.get_encoding();


    minfo.info._name = parser.get_name();
    minfo.info._piece_length = parser.get_plength();
    minfo.info._pieces = parser.get_pieces();

    if (parser.isSingleFile())
    {
        minfo.info.length = parser.get_size();
    }
    else
    {
        // For fiew files
    }

    return minfo;
}

//#include <iostream>
//#include <fstream>
inline std::string calculate_info_hash(MetaInfo minfo) 
{
    std::string str = 
        "d" +
        Bencoder::encode("length") + Bencoder::encode(minfo.info.length) + 
        Bencoder::encode("name") + Bencoder::encode(minfo.info._name) +
        Bencoder::encode("piece length") + Bencoder::encode(minfo.info._piece_length) +
        Bencoder::encode("pieces") + std::to_string(minfo.info._pieces.size()) + ":" + minfo.info._pieces +
        "e";

    /*
    std::ofstream myfile;
    myfile.open ("example.txt");
    myfile << str;
    myfile.close();
    //*/


    SHA1 checksum;
    checksum.update(str);

    return checksum.final();
}

/*
std::vector<std::string> get_pieces(const std::string& pieces_in_str)
{
    std::vector<std::string> result;

    for (int i = 0; i < pieces_in_str.size() / 20; ++i)
    {
        std::string str = pieces_in_str.substr(i * 20, (i + 1) * 20);
        char arr[41] = {'\0'};
        //answer.resize(41);

        for (int j = 0; j < 20; ++j)
        {
            char c = str[j];
            arr[j * 2] = (c >> 4) + '0';
            arr[j * 2 + 1] = (c & 0xf) + '0';
        }

        for (size_t j = 0; j < 40; ++j) {
            if (arr[j] > '9') 
            {
                arr[j] = arr[j] - '9' - 1 + 'a';
            }
        }

        std::cout << arr << "\n";

        result.push_back(arr);
    }

    return result;
}
*/

inline std::vector<std::string> get_pieces(const std::string& pieces_in_str)
{
    std::vector<std::string> result;

    for (int i = 0; i < pieces_in_str.size() / 20; ++i)
    {
        std::stringstream ss;
        auto piece = pieces_in_str.substr(i * 20, 20);

        for (unsigned char byte : piece)
        {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
        }

        result.push_back(ss.str());
    }
    
    return result;
}