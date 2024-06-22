

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

std::string calculate_info_hash(MetaInfo minfo) 
{
    std::string str = 
        Bencoder::encode("length") + Bencoder::encode(minfo.info.length) + 
        Bencoder::encode("name") + Bencoder::encode(minfo.info._name) +
        Bencoder::encode("piece length") + Bencoder::encode(minfo.info._piece_length) +
        Bencoder::encode("piece") + Bencoder::encode(minfo.info._pieces);

    std::cout << "\n\n\n\n" << str << "\n\n\n";

    SHA1 checksum;
    checksum.update(str);

    return checksum.final();
}