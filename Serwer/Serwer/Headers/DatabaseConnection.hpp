#ifndef DATABASE_CONNECTION_HPP
#define DATABASE_CONNECTION_HPP

#include <iostream>
#include <cstdio>
#include <vector>
#include <winsock.h>
#include <MySQL/mysql.h>
#include "Player.hpp"

class DatabaseConnection
{
    public:
    bool establishConnection(std::string host,std::string user,std::string password, std::string database);
    unsigned int checkAccount(std::string login,std::string password);
    Player getCharacter(unsigned int accountID,unsigned int characterID);
    int state;
    std::vector<std::string> getCharacterNames(unsigned int accountID);
    MYSQL *connection;
    MYSQL_ROW row;
    MYSQL_RES *result;
};

#endif // DATABASE_CONNECTION_HPP
