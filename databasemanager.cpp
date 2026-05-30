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