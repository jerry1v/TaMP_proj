#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "client.h"
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

Client::Client()
{
    clientSocket = INVALID_SOCKET;
}

Client& Client::getInstance()
{
    static Client instance;
    return instance;
}

bool Client::isConnected() const
{
    return clientSocket != INVALID_SOCKET;
}

bool Client::connectToServer()
{
    if (isConnected())
    {
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
    }

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == INVALID_SOCKET)
    {
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port   = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    int result = connect(
        clientSocket,
        (sockaddr*)&serverAddr,
        sizeof(serverAddr));

    if (result == SOCKET_ERROR)
    {
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
        return false;
    }

    return true;
}

QString Client::sendRequest(const QString& request)
{
    if (!isConnected())
    {
        if (!connectToServer())
        {
            return "CONNECTION_ERROR";
        }
    }

    QByteArray data = request.toUtf8();

    int sent = send(
        clientSocket,
        data.constData(),
        data.size(),
        0);

    if (sent == SOCKET_ERROR)
    {
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
        return "SEND_ERROR";
    }

    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));

    int len = recv(
        clientSocket,
        buffer,
        sizeof(buffer) - 1,
        0);

    if (len <= 0)
    {
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
        return "SERVER_ERROR";
    }

    buffer[len] = '\0';
    return QString::fromUtf8(buffer);
}

QString Client::registerUser(
    const QString& login,
    const QString& password)
{
    return sendRequest("REGISTER " + login + " " + password);
}

QString Client::loginUser(
    const QString& login,
    const QString& password)
{
    QString response = sendRequest("LOGIN " + login + " " + password);

    if (response.startsWith("LOGIN_SUCCESS"))
    {
        QStringList parts = response.split(" ");
        currentRole  = (parts.size() > 1) ? parts[1] : "USER";
        currentLogin = login;
    }

    return response;
}

QString Client::getUsersList()
{
    return sendRequest("USERS");
}

QString Client::deleteUser(const QString& login)
{
    return sendRequest("DELETE " + login);
}

QString Client::changeUserRole(
    const QString& login,
    const QString& newRole)
{
    return sendRequest("ROLE " + login + " " + newRole);
}

QString Client::getUserCount()
{
    return sendRequest("COUNT");
}
