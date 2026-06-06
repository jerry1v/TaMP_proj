#include "databasemanager.h"

DatabaseManager::DatabaseManager()
{
    User admin;

    admin.login = "admin";
    admin.password = "admin";
    admin.role = "ADMIN";

    users["admin"] = admin;
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
	user.role = "USER";

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

std::string DatabaseManager::getUsersList()
{
    std::string result;

    for (const auto& pair : users)
    {
        result += pair.second.login;
        result += " ";
        result += pair.second.role;
        result += "\n";
    }

    return result;
}

bool DatabaseManager::deleteUser(
    const std::string& login)
{
    auto it = users.find(login);

    if (it == users.end())
    {
        return false;
    }

    users.erase(it);

    return true;
}