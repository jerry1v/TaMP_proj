#pragma once

#include <string>
#include <map>
#include <vector>

struct User
{
    std::string login;
    std::string password;
    std::string role;
};

class DatabaseManager
{
private:
    std::map<std::string, User> users;
    std::string dbFilePath;

    DatabaseManager();

    void loadFromFile();
    void saveToFile();

public:
    static DatabaseManager& getInstance();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    bool registerUser(
        const std::string& login,
        const std::string& password);

    bool loginUser(
        const std::string& login,
        const std::string& password);

    std::string getUserRole(
        const std::string& login);

    std::string getUsersList();

    bool deleteUser(
        const std::string& login);

    bool changeUserRole(
        const std::string& login,
        const std::string& newRole);

    int getUserCount();
};
