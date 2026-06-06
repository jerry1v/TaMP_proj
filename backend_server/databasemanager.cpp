#include "databasemanager.h"
#include <fstream>
#include <sstream>
#include <iostream>

DatabaseManager::DatabaseManager()
{
    dbFilePath = "users.txt";

    // Загружаем пользователей из файла
    loadFromFile();

    // Если admin не существует — создаём
    if (users.find("admin") == users.end())
    {
        User admin;
        admin.login    = "admin";
        admin.password = "admin";
        admin.role     = "ADMIN";
        users["admin"] = admin;
        saveToFile();
    }
}

DatabaseManager& DatabaseManager::getInstance()
{
    static DatabaseManager instance;
    return instance;
}

void DatabaseManager::loadFromFile()
{
    std::ifstream file(dbFilePath);

    if (!file.is_open())
    {
        return;
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        std::istringstream ss(line);
        User user;

        if (std::getline(ss, user.login,    '|') &&
            std::getline(ss, user.password, '|') &&
            std::getline(ss, user.role))
        {
            if (!user.login.empty())
            {
                users[user.login] = user;
            }
        }
    }

    file.close();
}

void DatabaseManager::saveToFile()
{
    std::ofstream file(dbFilePath, std::ios::trunc);

    if (!file.is_open())
    {
        std::cerr << "ERROR: Cannot open users.txt for writing" << std::endl;
        return;
    }

    for (const auto& pair : users)
    {
        file << pair.second.login    << "|"
             << pair.second.password << "|"
             << pair.second.role     << "\n";
    }

    file.close();
}

bool DatabaseManager::registerUser(
    const std::string& login,
    const std::string& password)
{
    if (login.empty() || password.empty())
    {
        return false;
    }

    if (users.find(login) != users.end())
    {
        return false;
    }

    // Запрещаем спецсимволы в логине/пароле (разделитель |)
    if (login.find('|') != std::string::npos ||
        password.find('|') != std::string::npos)
    {
        return false;
    }

    User user;
    user.login    = login;
    user.password = password;
    user.role     = "USER";

    users[login] = user;
    saveToFile();

    return true;
}

bool DatabaseManager::loginUser(
    const std::string& login,
    const std::string& password)
{
    auto it = users.find(login);

    if (it == users.end())
    {
        return false;
    }

    return it->second.password == password;
}

std::string DatabaseManager::getUserRole(
    const std::string& login)
{
    auto it = users.find(login);

    if (it == users.end())
    {
        return "";
    }

    return it->second.role;
}

std::string DatabaseManager::getUsersList()
{
    std::string result;

    for (const auto& pair : users)
    {
        result += pair.second.login    + "|"
               +  pair.second.role     + "\n";
    }

    return result;
}

bool DatabaseManager::deleteUser(
    const std::string& login)
{
    if (login == "admin")
    {
        return false;  // нельзя удалить главного админа
    }

    auto it = users.find(login);

    if (it == users.end())
    {
        return false;
    }

    users.erase(it);
    saveToFile();

    return true;
}

bool DatabaseManager::changeUserRole(
    const std::string& login,
    const std::string& newRole)
{
    if (login == "admin")
    {
        return false;
    }

    auto it = users.find(login);

    if (it == users.end())
    {
        return false;
    }

    if (newRole != "USER" && newRole != "ADMIN")
    {
        return false;
    }

    it->second.role = newRole;
    saveToFile();

    return true;
}

int DatabaseManager::getUserCount()
{
    return (int)users.size();
}
