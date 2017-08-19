#pragma once

#include "../common/library.h"
#include "../common/stl.h"
#include "User.h"

//其它现在1v1, setUser只有一个
//这种设计不太好，还是要peer_id这样的设计，在一个session里的东西太乱
//不太有利于管理
class ClassSessionManager
{
public:
	ClassSessionManager();
	virtual ~ClassSessionManager();
	static ClassSessionManager& GetInstance();

public:
	void AddClassUser(string class_id, User* user, bool is_teacher);
	void RemoveUser(User* user);
	string GetClassForUser(User* user);
	set<User*> GetClassUsers(string class_id);
	User* GetClassTeacher(string class_id);

private:
	map<string, set<User*> > _classUsers;
	map<string, User*> _teachers;
};

