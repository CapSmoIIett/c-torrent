

#include "metainfo.h"
#include "file_parser.h"
#include "bencoder.h"
#include "sha1.h"


MetaInfo get_meta_info(TFileParser parser)
{
    MetaInfo minfo;    
    minfo.announce = parser.get_url();
    minfo.announce_list = parser.get_announce_list();
    minfo.creation_date = parser.get_creation_date();
    minfo.comment = parser.get_comment();
    minfo.created_by = parser.get_created_by();
    minfo.encoding = parser.get_encoding();


    minfo.info.length = parser.get_size();
    minfo.info._name = parser.get_name();
    minfo.info._piece_length = parser.get_plength();
    minfo.info._pieces = parser.get_pieces();

    return minfo;
}

//#include <iostream>
//#include <fstream>
std::string calculate_info_hash(MetaInfo minfo) 
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