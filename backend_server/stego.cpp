#include "stego.h"

std::string hideMessage(const std::string& data)
{
    return "MESSAGE_HIDDEN: " + data;
}