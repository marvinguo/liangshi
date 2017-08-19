#pragma once

#include "../common/stl.h"
#include "../common/library.h"
#include "../../../../SharedCode/LokiCommand.h"

class DatabaseEngine
{
private:
	DatabaseEngine();
	virtual ~DatabaseEngine();

public:
	static DatabaseEngine& GetInstance();
	bool IsConnected();
	bool Init();
	void Release();

public:
	bool CheckUserPassword(string& username, string& passwordMD5, int& user_id, string& display_name);
	bool GetUserFriendList(int user_id, vector<Loki::LokiFriend>& friends);
	void GetGrades();
	void GetGradeStudents();
	void GetGradeKes();
	bool IsTeacher(string& session_id, string& class_id, string& user_id, string& nickname, bool& hasVideo);
	bool IsStudent(string& session_id, string& class_id, string& user_id, string& nickname, bool& hasVideo, bool& allTeachers);

private:
	bool CheckConnection();

private:
	MySQL_Driver*_driver;
	Connection* _connection;

	//lock并不能很好的性能，可能需要pool, thread safe可能并不是
	//the database is running in multithread mode
	uv_rwlock_t _dblock;
};

