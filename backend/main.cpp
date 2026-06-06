#include "databasemanager.h"
#include <sstream>
#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

string parseRequest(const string& request)
{
    stringstream ss(request);

    string command;
    ss >> command;

    // ─── REGISTER login password ───────────────────────────────
    if (command == "REGISTER")
    {
        string login, password;
        ss >> login >> password;

        bool ok = DatabaseManager::getInstance()
                    .registerUser(login, password);

        return ok ? "REGISTER_SUCCESS" : "REGISTER_FAILED";
    }

    // ─── LOGIN login password ──────────────────────────────────
    if (command == "LOGIN")
    {
        string login, password;
        ss >> login >> password;

        bool ok = DatabaseManager::getInstance()
                    .loginUser(login, password);

        if (!ok) return "LOGIN_FAILED";

        string role = DatabaseManager::getInstance()
                        .getUserRole(login);

        return "LOGIN_SUCCESS " + role;
    }

    // ─── USERS  (список всех пользователей) ────────────────────
    if (command == "USERS")
    {
        string list = DatabaseManager::getInstance().getUsersList();
        return list.empty() ? "USERS_EMPTY" : list;
    }

    // ─── DELETE login ──────────────────────────────────────────
    if (command == "DELETE")
    {
        string login;
        ss >> login;

        bool ok = DatabaseManager::getInstance().deleteUser(login);
        return ok ? "DELETE_SUCCESS" : "DELETE_FAILED";
    }

    // ─── ROLE login newRole ────────────────────────────────────
    if (command == "ROLE")
    {
        string login, newRole;
        ss >> login >> newRole;

        bool ok = DatabaseManager::getInstance()
                    .changeUserRole(login, newRole);

        return ok ? "ROLE_SUCCESS" : "ROLE_FAILED";
    }

    // ─── COUNT (количество пользователей) ─────────────────────
    if (command == "COUNT")
    {
        int n = DatabaseManager::getInstance().getUserCount();
        return "COUNT " + to_string(n);
    }

    return "UNKNOWN_COMMAND";
}

void handleClient(SOCKET clientSocket)
{
    char buffer[4096];

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));

        int bytesReceived = recv(
            clientSocket,
            buffer,
            sizeof(buffer) - 1,
            0);

        if (bytesReceived <= 0)
        {
            break;
        }

        buffer[bytesReceived] = '\0';

        string request(buffer);
        string response = parseRequest(request);

        send(clientSocket,
             response.c_str(),
             (int)response.size(),
             0);
    }

    closesocket(clientSocket);
}

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == INVALID_SOCKET)
    {
        cerr << "Failed to create socket" << endl;
        return 1;
    }

    // Разрешаем переиспользование порта
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR,
               (char*)&opt, sizeof(opt));

    sockaddr_in serverAddr;
    serverAddr.sin_family      = AF_INET;
    serverAddr.sin_port        = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket,
             (sockaddr*)&serverAddr,
             sizeof(serverAddr)) == SOCKET_ERROR)
    {
        cerr << "Bind failed" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    listen(serverSocket, 10);

    cout << "SERVER_STARTED on port 8080" << endl;
    cout << "Users file: users.txt" << endl;

    while (true)
    {
        sockaddr_in clientAddr;
        int clientSize = sizeof(clientAddr);

        SOCKET clientSocket = accept(
            serverSocket,
            (sockaddr*)&clientAddr,
            &clientSize);

        if (clientSocket == INVALID_SOCKET)
        {
            continue;
        }

        thread clientThread(handleClient, clientSocket);
        clientThread.detach();
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
