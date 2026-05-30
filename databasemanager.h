#pragma once
#include <winsock2.h>
#include <string>
#include <map>

class DatabaseManager
{
private:
    std::map<std::string, std::string> storage;

    DatabaseManager();

public:
    static DatabaseManager& getInstance();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    void setValue(const std::string& key,
                  const std::string& value);

    std::string getValue(const std::string& key);
};