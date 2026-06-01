#include <cassert>

#include "../backend_server/databasemanager.h"

int main()
{
    DatabaseManager& db =
        DatabaseManager::getInstance();

    bool reg =
        db.registerUser(
            "admin",
            "123");

    assert(reg == true);

    bool login =
        db.loginUser(
            "admin",
            "123");

    assert(login == true);

    return 0;
}