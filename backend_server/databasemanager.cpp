#include "databasemanager.h"

DatabaseManager::DatabaseManager()
{
}

DatabaseManager& DatabaseManager::getInstance()
{
    static DatabaseManager instance;
    return instance;
}

void DatabaseManager::setValue(
    const std::string& key,
    const std::string& value)
{
    storage[key] = value;
}

std::string DatabaseManager::getValue(
    const std::string& key)
{
    auto it = storage.find(key);

    if (it != storage.end())
        return it->second;

    return "";
}

bool DatabaseManager::registerUser(
    const std::string& login,
    const std::string& password)
{
    if (users.find(login) != users.end())
    {
        return false;
    }

    User user;
    user.login = login;
    user.password = password;

    users[login] = user;

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