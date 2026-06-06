#include "aes.h"

std::string aesEncrypt(const std::string& data)
{
    std::string result = data;

    const std::string key = "AESKEY";

    for (size_t i = 0; i < result.size(); i++)
    {
        result[i] ^= key[i % key.size()];
    }

    return result;
}