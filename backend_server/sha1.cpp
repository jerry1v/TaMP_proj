#include "sha1.h"
#include <cstdint>
#include <sstream>
#include <iomanip>

static uint32_t rotl(uint32_t val, int shift)
{
    return (val << shift) | (val >> (32 - shift));
}

std::string sha1Hash(const std::string& text)
{
    uint32_t h0 = 0x67452301u;
    uint32_t h1 = 0xEFCDAB89u;
    uint32_t h2 = 0x98BADCFEu;
    uint32_t h3 = 0x10325476u;
    uint32_t h4 = 0xC3D2E1F0u;

    // Pre-processing: adding padding bits
    std::string msg = text;
    uint64_t bitLen = (uint64_t)text.size() * 8;
    msg.push_back((char)0x80);
    while (msg.size() % 64 != 56)
        msg.push_back(0x00);

    for (int i = 7; i >= 0; i--)
        msg.push_back((char)((bitLen >> (i * 8)) & 0xFF));

    // Process each 512-bit chunk
    for (size_t offset = 0; offset < msg.size(); offset += 64)
    {
        uint32_t w[80];
        for (int i = 0; i < 16; i++)
        {
            w[i] = ((uint8_t)msg[offset + i*4]     << 24) |
                   ((uint8_t)msg[offset + i*4 + 1] << 16) |
                   ((uint8_t)msg[offset + i*4 + 2] <<  8) |
                   ((uint8_t)msg[offset + i*4 + 3]);
        }
        for (int i = 16; i < 80; i++)
            w[i] = rotl(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);

        uint32_t a = h0, b = h1, c = h2, d = h3, e = h4;

        for (int i = 0; i < 80; i++)
        {
            uint32_t f, k;
            if      (i < 20) { f = (b & c) | (~b & d); k = 0x5A827999u; }
            else if (i < 40) { f = b ^ c ^ d;           k = 0x6ED9EBA1u; }
            else if (i < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDCu; }
            else             { f = b ^ c ^ d;           k = 0xCA62C1D6u; }

            uint32_t tmp = rotl(a, 5) + f + e + k + w[i];
            e = d; d = c; c = rotl(b, 30); b = a; a = tmp;
        }

        h0 += a; h1 += b; h2 += c; h3 += d; h4 += e;
    }

    std::ostringstream oss;
    oss << std::hex << std::setfill('0')
        << std::setw(8) << h0
        << std::setw(8) << h1
        << std::setw(8) << h2
        << std::setw(8) << h3
        << std::setw(8) << h4;
    return oss.str();
}
