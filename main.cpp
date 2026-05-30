// main.cpp
// Простой сервер-заглушка для обработки запросов
// Windows + C++

// Компиляция:
// g++ main.cpp -lws2_32 -o server.exe

#include <iostream>
#include <string>
#include <winsock2.h>

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

void parseRequest(const string& request)
{
    cout << "Получен запрос: " << request << endl;

    // AES
    if (request.find("AES ") == 0)
    {
        string data = request.substr(4);
        aesEncrypt(data);
    }

    // SHA1
    else if (request.find("SHA1 ") == 0)
    {
        string data = request.substr(5);
        sha1Hash(data);
    }

    // NEWTON
    else if (request == "NEWTON")
    {
        newtonMethod();
    }

    // STEGO
    else if (request.find("STEGO ") == 0)
    {
        string data = request.substr(6);
        hideMessage(data);
    }

    else
    {
        cout << "Неизвестная команда" << endl;
    }
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
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);

    cout << "Клиент подключен!" << endl;

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));

        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived <= 0)
        {
            cout << "Клиент отключился" << endl;
            break;
        }

        string request(buffer);

        parseRequest(request);

        string response = "OK";
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    closesocket(clientSocket);
    closesocket(serverSocket);

    WSACleanup();

    return 0;
}

// folk cat