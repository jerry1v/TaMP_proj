#include "sha1.h"

std::string sha1Hash(const std::string& text)
{
    unsigned long hash = 0;

    for (char c : text)
    {
        hash = hash * 31 + c;
    }

    return std::to_string(hash);
}