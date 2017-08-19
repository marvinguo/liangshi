#pragma once

//配置先写死，有可能需要配置文件或参数
#define SERVER_ADDR "0.0.0.0" // a.k.a. "all interfaces"
#define SERVER_PORT 8169

#ifdef _DEBUG
	#define DB_HOST "127.0.0.1"
	#define DB_USER "root"
	#define DB_USER_PASSWORD ""
	#define DB_NAME "fun1"
	#define DB_CONNECTION_STR "tcp://127.0.0.1:3306/fun1"
#else
	#define DB_HOST "rdsq3qmbzq3qmbz.mysql.rds.aliyuncs.com"
	#define DB_USER "dbs6ijxw2up14ihg"
	#define DB_USER_PASSWORD "pass123"
	#define DB_NAME "dbs6ijxw2up14ihg"
	#define DB_CONNECTION_STR "tcp://rdsq3qmbzq3qmbz.mysql.rds.aliyuncs.com/dbs6ijxw2up14ihg"
#endif 

