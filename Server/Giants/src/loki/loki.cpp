#include "mysql_api.h"

MySQL::MySQL()
{
    mysql_init(&_mysql);
    _host = "localhost";
    _user = "root";
    _password = "";
    _port = 3306;
    _db = "";
}

MySQL::MySQL(std::string host, std::string user, std::string password, std::string db = NULL, int port = 3306)
{
    mysql_init(&_mysql);
    _host = host;
    _user = user;
    _password = password;
    _db = db;
    _port = port;
}

MySQL::~MySQL()
{
    disconnect();
}

bool MySQL::connect()
{
    if (!mysql_real_connect(&_mysql, _host.c_str(), _user.c_str(), _password.c_str(), _db.c_str(), _port, NULL, 0))
    {
        return false;
    }
    return true;
}

bool MySQL::connect(std::string host, std::string user, std::string password, std::string db = NULL, int port = 3306)
{
    if (!mysql_real_connect(&_mysql, host.c_str(), user.c_str(), password.c_str(), db.c_str(), port, NULL, 0))
    {
        return false;
    }
    return true;
}

void MySQL::setCharacterSet(std::string characterSet)
{
    mysql_options(&_mysql, MYSQL_SET_CHARSET_NAME, characterSet.c_str());
}

void MySQL::disconnect()
{
    if (isConnected())
    {
        mysql_close(&_mysql);
    }
}

/* Error reporting */
unsigned int MySQL::getErrorCode()
{
    return mysql_errno(&_mysql);
}

std::string MySQL::getErrorString()
{
    return (const char *)mysql_error(&_mysql);
}
/* Error reporting */

bool MySQL::isConnected()
{
    return (mysql_ping(&_mysql) == 0);
}

bool MySQL::setecltDB(std::string db)
{
    return (mysql_select_db(&_mysql, db.c_str()) == 0);
}

std::string MySQL::escapeString(std::string what)
{
    if (!isConnected())
        return what;
    char *to = new char[(what.size() * 2) + 1];
    mysql_real_escape_string(&_mysql, to, what.c_str(), what.size());
    return (const char*)to;
}

bool MySQL::query(std::string query)
{
    if (mysql_real_query(&_mysql, query.c_str(), query.size()))
        return false;
    if (mysql_field_count(&_mysql)) // This is SELECT-like statement. We need to store result.
    {
        _result = MySQL_Query(mysql_store_result(&_mysql));
    }
    else
    {
        _result = MySQL_Query();
    }
    return true;
}

bool MySQL::transaction()
{
    return (mysql_autocommit(&_mysql, 0) == 0);
}

bool MySQL::commit()
{
    if (mysql_commit(&_mysql))
        return false;
    if (mysql_autocommit(&_mysql, 1))
        return false;
    return true;
}

bool MySQL::rollback()
{
    if (mysql_rollback(&_mysql))
        return false;
    if (mysql_autocommit(&_mysql, 1))
        return false;
    return true;
}

MySQL_Query MySQL::getResult()
{
    return _result;
}
