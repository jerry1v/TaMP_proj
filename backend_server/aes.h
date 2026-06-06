#pragma once
#include <string>

std::string aesEncrypt(const std::string& text,
                       const std::string& key = "defaultkey");

std::string aesDecrypt(const std::string& hexCipher,
                       const std::string& key = "defaultkey");
