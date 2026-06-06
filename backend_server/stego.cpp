#include "stego.h"
#include <cstdint>
#include <stdexcept>

static const int WAV_HEADER_SIZE = 44;

std::vector<char> stegoHide(const std::vector<char>& wavData,
                             const std::string& message)
{
    if ((int)wavData.size() < WAV_HEADER_SIZE) return {};

    // Minimal WAV validation
    if (wavData[0]!='R'||wavData[1]!='I'||wavData[2]!='F'||wavData[3]!='F') return {};
    if (wavData[8]!='W'||wavData[9]!='A'||wavData[10]!='V'||wavData[11]!='E') return {};

    uint32_t msgLen     = (uint32_t)message.size();
    int      bitsNeeded = (int)(4 + msgLen) * 8;
    int      available  = (int)wavData.size() - WAV_HEADER_SIZE;

    if (available < bitsNeeded) return {};

    // Build payload: 4-byte length prefix + message bytes
    std::vector<unsigned char> payload;
    payload.push_back((msgLen >> 24) & 0xFF);
    payload.push_back((msgLen >> 16) & 0xFF);
    payload.push_back((msgLen >>  8) & 0xFF);
    payload.push_back( msgLen        & 0xFF);
    for (char c : message)
        payload.push_back((unsigned char)c);

    std::vector<char> result = wavData;

    int bitIndex = 0;
    for (unsigned char byte : payload)
    {
        for (int bit = 7; bit >= 0; bit--)
        {
            int pos = WAV_HEADER_SIZE + bitIndex;
            unsigned char sample = (unsigned char)result[pos];
            sample = (sample & 0xFE) | ((byte >> bit) & 1);
            result[pos] = (char)sample;
            bitIndex++;
        }
    }

    return result;
}

std::string stegoExtract(const std::vector<char>& wavData)
{
    if ((int)wavData.size() < WAV_HEADER_SIZE + 32) return {};
    if (wavData[0]!='R'||wavData[1]!='I'||wavData[2]!='F'||wavData[3]!='F') return {};

    // Read 32 bits → message length
    uint32_t msgLen = 0;
    for (int bit = 0; bit < 32; bit++)
    {
        unsigned char sample = (unsigned char)wavData[WAV_HEADER_SIZE + bit];
        msgLen = (msgLen << 1) | (sample & 1);
    }

    if (msgLen == 0 || msgLen > 100000) return {};

    int available = (int)wavData.size() - WAV_HEADER_SIZE;
    if (available < (int)(32 + msgLen * 8)) return {};

    std::string result;
    for (uint32_t i = 0; i < msgLen; i++)
    {
        unsigned char b = 0;
        for (int bit = 0; bit < 8; bit++)
        {
            int pos = WAV_HEADER_SIZE + 32 + (int)(i * 8 + bit);
            unsigned char sample = (unsigned char)wavData[pos];
            b = (b << 1) | (sample & 1);
        }
        result.push_back((char)b);
    }

    return result;
}
