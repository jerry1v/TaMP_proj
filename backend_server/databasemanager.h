#pragma once

#include <string>
#include <map>

struct User
{
    std::string login;
    std::string password;
    std::string role;
}

/**
 * @brief Singleton база данных сервера.
 *
 * Хранит пользователей и служебные данные.
 */
class DatabaseManager
{
private:
    std::map<std::string, std::string> storage;
    std::map<std::string, User> users;

    DatabaseManager();

public:
	/**
	* @brief Получить экземпляр Singleton.
	*/
    static DatabaseManager& getInstance();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    void setValue(const std::string& key,
                  const std::string& value);

    std::string getValue(const std::string& key);
	/**
	* @brief Регистрация пользователя.
	* @param login Логин
	* @param password Пароль
	* @return true если успешно
	*/
    bool registerUser(
        const std::string& login,
        const std::string& password);
	/**
	* @brief Авторизация пользователя.
	* @param login Логин
	* @param password Пароль
	* @return true если данные верны
	*/
    bool loginUser(
        const std::string& login,
        const std::string& password);
	std::string getUserRole(
		std::string& login);
}