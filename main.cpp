// main.cpp
// Простой сервер-заглушка для обработки запросов
// Windows + C++

// Компиляция:
// g++ main.cpp -lws2_32 -o server.exe

#include "databasemanager.h"
#include <sstream>
#include <iostream>
#include <string>
#include <winsock2.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

// =======================
// Заглушки функций
// =======================

void aesEncrypt(const string& data)
{
    DatabaseManager::getInstance()
        .setValue("last_aes", data);

    cout << "[AES] Шифрование: "
         << data << endl;
}

void sha1Hash(const string& data)
{
    DatabaseManager::getInstance()
        .setValue("last_sha1", data);

    cout << "[SHA1] Хэширование: "
         << data << endl;
}

void newtonMethod()
{
    DatabaseManager::getInstance()
        .setValue("last_method", "NEWTON");

    cout << "[NEWTON] Метод Ньютона вызван"
         << endl;
}

void hideMessage(const string& data)
{
    DatabaseManager::getInstance()
        .setValue("last_stego", data);

    cout << "[STEGO] Внедрение сообщения: "
         << data << endl;
}

// =======================
// Парсер запросов
// =======================

string parseRequest(const string& request)
{
    cout << "Получен запрос: " << request << endl;

    stringstream ss(request);

    string command;
    ss >> command;

    if (command == "REGISTER")
    {
        string login;
        string password;

        ss >> login;
        ss >> password;

        bool result =
            DatabaseManager::getInstance()
            .registerUser(login, password);

        return result ?
            "REGISTER_SUCCESS" :
            "REGISTER_FAILED";
    }

    if (command == "LOGIN")
    {
        string login;
        string password;

        ss >> login;
        ss >> password;

        bool result =
            DatabaseManager::getInstance()
            .loginUser(login, password);

        return result ?
            "LOGIN_SUCCESS" :
            "LOGIN_FAILED";
    }

    if (command == "AES")
    {
        return "AES_STUB";
    }

    if (command == "SHA1")
    {
        return "SHA1_STUB";
    }

    if (command == "NEWTON")
    {
        return "NEWTON_STUB";
    }

    if (command == "STEGO")
    {
        return "STEGO_STUB";
    }

    return "UNKNOWN_COMMAND";
}

void handleClient(SOCKET clientSocket)
{
    char buffer[1024];

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));

        int bytesReceived =
            recv(clientSocket,
                 buffer,
                 sizeof(buffer),
                 0);

        if (bytesReceived <= 0)
        {
            cout << "Клиент отключился" << endl;
            break;
        }

        string request(buffer);

        string response =
			parseRequest(request);

        send(clientSocket,
             response.c_str(),
             response.size(),
             0);
    }

    closesocket(clientSocket);
}

// =======================
// Основной сервер
// =======================

int main()
{
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;

    int clientSize = sizeof(clientAddr);
    char buffer[1024];

    // Инициализация Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Создание сокета
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == INVALID_SOCKET)
    {
        cout << "Ошибка создания сокета" << endl;
        return 1;
    }

    // Настройки сервера
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    // Listen
    listen(serverSocket, 5);

    cout << "Сервер запущен на порту 8080..." << endl;

    // Accept
    while (true)
{
    clientSocket =
        accept(serverSocket,
               (sockaddr*)&clientAddr,
               &clientSize);

    if (clientSocket == INVALID_SOCKET)
    {
        continue;
    }

    cout << "Подключен новый клиент"
         << endl;

    thread clientThread(
        handleClient,
        clientSocket);

    clientThread.detach();
}
    closesocket(clientSocket);
    closesocket(serverSocket);

    WSACleanup();

    return 0;
}

// folk cat