#pragma once

#include <string>
#include <map>

struct User
{
    std::string login;
    std::string password;
};

class DatabaseManager
{
private:
    std::map<std::string, std::string> storage;
    std::map<std::string, User> users;

    DatabaseManager();

public:
    static DatabaseManager& getInstance();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    void setValue(const std::string& key,
                  const std::string& value);

    std::string getValue(const std::string& key);

    bool registerUser(
        const std::string& login,
        const std::string& password);

    bool loginUser(
        const std::string& login,
        const std::string& password);
};