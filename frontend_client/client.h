#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <winsock2.h>

class Client
{
private:
    SOCKET clientSocket;

    Client();

public:
    static Client& getInstance();

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    bool connectToServer();

    QString sendRequest(
        const QString& request);

    QString registerUser(
        const QString& login,
        const QString& password);

    QString loginUser(
        const QString& login,
        const QString& password);
		
	QString sendNewton(double number);
};

#endif