#include "MysqlWrapper.h"

MysqlWrapper::MysqlWrapper():
_result(NULL)
{
	mysql_init(&_mysqlHandler);
}

MysqlWrapper::~MysqlWrapper()
{
	Close();
}

bool MysqlWrapper::IsConnect()
{
	return (mysql_ping(&_mysqlHandler) == 0);
}

bool MysqlWrapper::Connect(const char* HostName,const char* UserName,const char* Password,const char* DataBase)
{
	_isConnected = false;
//#ifdef WIN32
//	mysql_connect(&Mysql,HostName,UserName,Password);
	mysql_real_connect(&Mysql,HostName,UserName,Password,DataBase,0,NULL,0);
	if(Mysql.net.last_errno!=0)
	{
		printf("###### MysqlDB Connect fail.last_errno:%u error:%s.\r\n", Mysql.net.last_errno, Mysql.net.last_error);
		return false;
	}
/*
	mysql_select_db(&Mysql, DataBase);	//if successfully select db,return 0.
	if(Mysql.net.last_errno!=0)
	{
		return false;
	}
	*/
	/*
#else
	mysql_real_connect(&Mysql,HostName,UserName,Password,DataBase,0,NULL,0);
	if(Mysql.net.last_errno!=0)
	{
		return false;
	}
#endif
	*/

	Connected=true;
	return true;	//if successfully connected to db, return 0,
						//error message equals to Rec.net.last_error
}

//数据库关闭
bool MysqlWrapper::Close()
{
	SQLClose();

	bool bCloseStatus=false;
	if(Connected==true)
	{
		mysql_close(&Mysql);
#if 0 // 2010.06.07
		// 2010.05.20
		mysql_thread_end();
		// end.
#endif
		if(Mysql.net.last_errno==0)
		{
			return true;
		}
	}
	return bCloseStatus;
}

//执行SQL语句
bool MysqlWrapper::SQLExec(const char *SQL)
{
	SQLClose();

	mysql_query(&Mysql,SQL);
	if(Mysql.net.last_errno!=0)
	{
		printf("###### MysqlDB SQL mysql_query fail.last_errno:%u error:%s.\r\n", Mysql.net.last_errno, Mysql.net.last_error);
		return false;
	}
	Result = mysql_store_result(&Mysql); //Store results locally
	if(Mysql.net.last_errno!=0)
	{
		printf("###### MysqlDB SQL mysql_store_result fail.last_errno:%u error:%s.\r\n", Mysql.net.last_errno, Mysql.net.last_error);
		return false;
	}
	return true;  //if successfully connected to db, return 0,
				  //error message equals to Rec.net.last_error
}

unsigned long MysqlWrapper::AffectedRows()
{	 
	unsigned long rows=(unsigned long)mysql_affected_rows(&Mysql);
	if(Mysql.net.last_errno!=0)
	{
		return 0;
	}
	return rows;
}

bool MysqlWrapper::SQLClose()
{
	if (Result)
	{
		mysql_free_result(Result);	//release the stored results from memory
		Result=NULL;
	}

	if(Mysql.net.last_errno!=0)
	{
		return false;
	}
	return true;  //if successfully connected to db, return 0,
								//error message equals to Rec.net.last_error
}

unsigned long MysqlWrapper::RecordCount()
{
	unsigned long ulCount = (unsigned long)mysql_num_rows(Result);
	 
	if(Mysql.net.last_errno!=0)
	{
		return ulCount=0;
	}
	return ulCount;
}

MYSQL_ROW MysqlWrapper::FetchRows()
{
	MYSQL_ROW tmpRow;

	tmpRow=mysql_fetch_row(Result);

	if(Mysql.net.last_errno!=0)
	{
		return NULL;
	}
	return tmpRow;
}
