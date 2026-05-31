#ifndef CLIENT_H
#define CLIENT_H

#include <QString>

class Client
{
private:
    Client();

public:
    static Client& getInstance();

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    bool connectToServer();

    QString registerUser(
        const QString& login,
        const QString& password);

    QString loginUser(
        const QString& login,
        const QString& password);

    QString sendAES(
        const QString& text);

    QString sendSHA1(
        const QString& text);

    QString sendNewton();

    QString sendStego(
        const QString& text);
};

#endif