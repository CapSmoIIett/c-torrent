#include "metainfo.h"



MetaInfo::MetaInfo(const TFileParser& parser)
{

    announce = parser.get_url();
    announce_list;
    creation_date = parser.get_creation_date();
    comment = parser.get_field("comment");
    created_by;
    encoding = parser.get_encoding();

}