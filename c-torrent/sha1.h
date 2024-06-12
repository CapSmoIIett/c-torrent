#pragma once

#include <string>

class SHA1 final
{
public:
    static std::string algo(std::string);

private:
    static const uint32_t constants[5];
};