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

bool Client::connectToServer()
{
    WSADATA wsaData;

    WSAStartup(
        MAKEWORD(2, 2),
        &wsaData);

    clientSocket =
        socket(AF_INET,
               SOCK_STREAM,
               0);

    if (clientSocket == INVALID_SOCKET)
    {
        return false;
    }

    sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &serverAddr.sin_addr);

    int result =
        connect(
            clientSocket,
            (sockaddr*)&serverAddr,
            sizeof(serverAddr));

    return result != SOCKET_ERROR;
}

QString Client::sendRequest(
    const QString& request)
{
    send(
        clientSocket,
        request.toStdString().c_str(),
        request.size(),
        0);

    char buffer[1024];

    memset(buffer, 0, sizeof(buffer));

    recv(
        clientSocket,
        buffer,
        sizeof(buffer),
        0);

    return QString(buffer);
}

QString Client::registerUser(
    const QString& login,
    const QString& password)
{
    return sendRequest(
        "REGISTER " +
        login +
        " " +
        password);
}

QString Client::loginUser(
    const QString& login,
    const QString& password)
{
    return sendRequest(
        "LOGIN " +
        login +
        " " +
        password);
		
	if (response.startsWith("LOGIN_SUCCESS"))
	{
		QStringList parts =
			response.split(" ");

		if (parts.size() > 1)
		{
			currentRole = parts[1];
		}
	}
}

QString Client::sendNewton(double number)
{
    return sendRequest(
        "NEWTON " +
        QString::number(number));
}		

QString Client::getRole() const
{
    return currentRole;
}