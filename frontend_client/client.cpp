#include "client.h"

Client::Client()
{
}

Client& Client::getInstance()
{
    static Client instance;
    return instance;
}

bool Client::connectToServer()
{
    return true;
}

QString Client::registerUser(
    const QString& login,
    const QString& password)
{
    return "REGISTER_SUCCESS";
}

QString Client::loginUser(
    const QString& login,
    const QString& password)
{
    return "LOGIN_SUCCESS";
}

QString Client::sendAES(
    const QString& text)
{
    return "AES_STUB";
}

QString Client::sendSHA1(
    const QString& text)
{
    return "SHA1_STUB";
}

QString Client::sendNewton()
{
    return "NEWTON_STUB";
}

QString Client::sendStego(
    const QString& text)
{
    return "STEGO_STUB";
}