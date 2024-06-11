#pragma once

#include <string>

class SHA1 final
{
public:
    static std::string algo(std::string);

private:
    uint32_t constants[5] =
    {
        0x67452301,
        0xEFCDAB89,
        0x98BADCFE,
        0x10325476,
        0xC3D2E1F0
    };
};