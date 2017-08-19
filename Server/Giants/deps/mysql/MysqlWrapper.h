#pragma once
#include "mysql.h"

class MysqlWrapper
{
public:
	MysqlWrapper();
	virtual ~MysqlWrapper();

	bool IsConnect();
	bool Connect(const char* HostName,const char* UserName,const char* Password,const char* DataBase);
	bool SQLExec(const char *SQL);
	//update,insert,delete的影响行数
	unsigned long AffectedRows();

	//关闭一个Result(查询结果集合)
	bool SQLClose();
	//返回查询select的数目
	unsigned long RecordCount();
	//取下一行
	MYSQL_ROW FetchRows();
	bool Close();

private:
	MYSQL _mysqlHandler;
	MYSQL_RES *_result;
	MYSQL_ROW _rows;
	bool _isConnected;
};

