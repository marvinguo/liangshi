#ifndef MYSQL_API_H
#define MYSQL_API_H

#include <iostream>
#include <string>

#include "mysql_query.h"

#include <mysql.h>

class MySQL
{
public:
    explicit MySQL();
    explicit MySQL(std::string host, std::string user, std::string password, std::string db, int port);
    ~MySQL();

    bool connect();
    bool connect(std::string host, std::string user, std::string password, std::string db, int port);
    void disconnect();

    std::string getErrorString();
    unsigned int getErrorCode();

    void setCharacterSet(std::string characterSet);
    bool isConnected();

    bool setecltDB(std::string db);

    // Transaction
    bool transaction();
    bool commit();
    bool rollback();
    // Transaction

    std::string escapeString(std::string what);

    bool query(std::string query);

    MySQL_Query getResult();

private:
    MYSQL _mysql;
    std::string _host;
    int _port;
    std::string _user;
    std::string _password;
    std::string _db;

    MySQL_Query _result;
};

#endif // MYSQL_API_H
