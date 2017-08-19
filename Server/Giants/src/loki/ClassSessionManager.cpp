#include "ClassSessionManager.h"
#include "Console.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ClassSessionManager& ClassSessionManager::GetInstance()
{
	static ClassSessionManager manager;
	return manager;
}

ClassSessionManager::ClassSessionManager()
{
}

ClassSessionManager::~ClassSessionManager()
{
}

void ClassSessionManager::AddClassUser(string class_id, User* user, bool is_teacher)
{
	map<string, set<User*> >::iterator it;
	set<User*> setUser;
	it = _classUsers.find(class_id);
	if(it != _classUsers.end())
	{
		setUser.insert(it->second.begin(), it->second.end());
		//copy((*it).second()->begin(), (*it).second->end()), setUser.begin());
	}
	else
	{
		//_users.insert(pair<string, set<User*> >(session, setUser));
	}
	setUser.insert(user);
	_classUsers[class_id] = setUser;

	if(is_teacher)
	{
		_teachers[class_id] = user;
	}
}

void ClassSessionManager::RemoveUser(User* user)
{
	for(map<string, set<User*> >::iterator it = _classUsers.begin(); it != _classUsers.end(); ++it)
	{
		it->second.erase(user);
	}

	for(map<string, User*>::iterator it = _teachers.begin(); it != _teachers.end(); ++it)
	{
		if(it->second == user)
		{
			_teachers.erase(it);
			break;
		}
	}
}

string ClassSessionManager::GetClassForUser(User* user)
{
	return "";
}

set<User*> ClassSessionManager::GetClassUsers(string class_id)
{
	map<string, set<User*> >::iterator it;
	set<User*> setUser;
	it = _classUsers.find(class_id);
	if(it != _classUsers.end())
	{
		setUser.insert(it->second.begin(), it->second.end());
		//copy((*it).second()->begin(), (*it).second->end()), setUser.begin());
	}
	return setUser;
}

User* ClassSessionManager::GetClassTeacher(string class_id)
{
	User* teacher = NULL;
	map<string, User*>::iterator it;
	it = _teachers.find(class_id);
	if(it != _teachers.end())
	{
		teacher = it->second;
	}
	return teacher;
}