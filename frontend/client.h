#pragma once

// Qt headers MUST come before winsock to avoid conflicts
#include <QString>
#include <QStringList>

// Prevent windows.h from including winsock 1.x before winsock2
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>

class Client
{
private:
    SOCKET clientSocket;
    QString currentRole;
    QString currentLogin;

    Client();

public:
    static Client& getInstance();

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    bool connectToServer();
    bool isConnected() const;

    QString sendRequest(const QString& request);

    QString registerUser(const QString& login, const QString& password);
    QString loginUser(const QString& login, const QString& password);
    QString getUsersList();
    QString deleteUser(const QString& login);
    QString changeUserRole(const QString& login, const QString& newRole);
    QString getUserCount();

    QString getCurrentRole()  const { return currentRole;  }
    QString getCurrentLogin() const { return currentLogin; }
};
