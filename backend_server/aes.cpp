#include "aes.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

static std::string toHex(const std::string& bytes)
{
    std::ostringstream oss;
    for (unsigned char c : bytes)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    return oss.str();
}

static std::string fromHex(const std::string& hex)
{
    if (hex.size() % 2 != 0)
        throw std::invalid_argument("Hex string length must be even");

    std::string result;
    for (size_t i = 0; i < hex.size(); i += 2)
    {
        unsigned int byte;
        std::istringstream ss(hex.substr(i, 2));
        ss >> std::hex >> byte;
        result.push_back(static_cast<char>(byte));
    }
    return result;
}

std::string aesEncrypt(const std::string& text, const std::string& key)
{
    std::string k = key.empty() ? "defaultkey" : key;
    std::string result(text.size(), '\0');
    for (size_t i = 0; i < text.size(); i++)
        result[i] = text[i] ^ k[i % k.size()];
    return toHex(result);
}

std::string aesDecrypt(const std::string& hexCipher, const std::string& key)
{
    std::string k    = key.empty() ? "defaultkey" : key;
    std::string data = fromHex(hexCipher);
    std::string result(data.size(), '\0');
    for (size_t i = 0; i < data.size(); i++)
        result[i] = data[i] ^ k[i % k.size()];
    return result;
}
