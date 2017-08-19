#include "DatabaseEngine.h"
#include "../common/config.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

DatabaseEngine& DatabaseEngine::GetInstance()
{
	static DatabaseEngine engine;
	return engine;
}

DatabaseEngine::DatabaseEngine():
_driver(get_mysql_driver_instance()),
_connection(NULL)
{
	uv_rwlock_init(&_dblock);
}

DatabaseEngine::~DatabaseEngine()
{
	if(_connection != NULL)
		delete _connection;
}

bool DatabaseEngine::IsConnected()
{
	return !_connection->isClosed();
}

bool DatabaseEngine::Init()
{
	try
	{
		if(_connection != NULL)
			delete _connection;

		_connection = _driver->connect(DB_CONNECTION_STR, DB_USER, DB_USER_PASSWORD);
		//_connection->setSchema(DB_NAME);
	}
	catch (SQLException &ex)
	{
		printf("Mysql Failed to connect: %s", ex.getSQLStateCStr());
		return false;
	}

	return true;
}

void DatabaseEngine::Release()
{
	_connection->close();
	delete _connection;
	_connection = NULL;
}

bool DatabaseEngine::CheckConnection()
{
	if(!IsConnected())
	{
		return Init();
	}
	return true;
}

bool DatabaseEngine::CheckUserPassword(string& username, string& passwordMD5, int& user_id, string& display_name)
{
	if(!CheckConnection())
		return false;
	SQLString sql = "select user_id, display_name from user where login_name = ? and password = ?";
	auto_ptr<PreparedStatement> stat(_connection->prepareStatement(sql));
	stat->setString(1, username);
	stat->setString(2, passwordMD5);

	uv_rwlock_rdlock(&_dblock);
	auto_ptr<ResultSet> ret(stat->executeQuery());
	uv_rwlock_rdunlock(&_dblock);

	while (ret->next())
	{
		user_id = ret->getInt("user_id");
		display_name = ret->getString("display_name");
		return true;
	}
	return false;
}

bool DatabaseEngine::GetUserFriendList(int user_id, vector<Loki::LokiFriend>& friends)
{
	if(!CheckConnection())
		return false;
	SQLString sql = "select friend_id, display_name, comment_name, group_name from user_friend left join user on user_friend.friend_id = user.user_id where user_friend.user_id = ?";
	auto_ptr<PreparedStatement> stat(_connection->prepareStatement(sql));
	stat->setInt(1, user_id);

	uv_rwlock_rdlock(&_dblock);
	auto_ptr<ResultSet> ret(stat->executeQuery());
	uv_rwlock_rdunlock(&_dblock);

	while (ret->next())
	{
		Loki::LokiFriend aFriend;
		aFriend.user_id = ret->getInt("friend_id");
		aFriend.display_name = ret->getString("display_name");
		aFriend.comment_name = ret->getString("comment_name");
		aFriend.group_name = ret->getString("group_name");
		friends.push_back(aFriend);
	}
	return true;
}

void DatabaseEngine::GetGrades()
{

}

void DatabaseEngine::GetGradeStudents()
{

}

//这里需要用云存储，返回uuid的信息
void DatabaseEngine::GetGradeKes()
{

}

bool DatabaseEngine::IsTeacher(string& session_id, string& class_id, string& user_id, string& nickname, bool& hasVideo)
{
	if(!CheckConnection())
		return false;
	SQLString sql = "select class.class_id, class.user_id, class.has_video, user.nickname from class, user where user.user_id = class.user_id and class.session_id = ?";
	auto_ptr<PreparedStatement> stat(_connection->prepareStatement(sql));
	stat->setString(1, session_id);

	uv_rwlock_rdlock(&_dblock);
	auto_ptr<ResultSet> ret(stat->executeQuery());
	uv_rwlock_rdunlock(&_dblock);

	bool is_teacher = false;

	while (ret->next())
	{
		class_id = ret->getString("class_id");
		user_id = ret->getString("user_id");
		nickname = ret->getString("nickname");
		hasVideo = ret->getBoolean("has_video");
		is_teacher = true;
	}
	return is_teacher;
}

bool DatabaseEngine::IsStudent(string& session_id, string& class_id, string& user_id, string& nickname, bool& hasVideo, bool& allTeachers)
{
	if(!CheckConnection())
		return false;
	SQLString sql = "select class_student.class_id, class_student.user_id, user.nickname, class.all_teachers, class.has_video from class, class_student, user where user.user_id = class_student.user_id and class.class_id = class_student.class_id and class_student.session_id = ?";
	auto_ptr<PreparedStatement> stat(_connection->prepareStatement(sql));
	stat->setString(1, session_id);

	uv_rwlock_rdlock(&_dblock);
	auto_ptr<ResultSet> ret(stat->executeQuery());
	uv_rwlock_rdunlock(&_dblock);

	bool is_student = false;

	while (ret->next())
	{
		class_id = ret->getString("class_id");
		user_id = ret->getString("user_id");
		nickname = ret->getString("nickname");
		hasVideo = ret->getBoolean("has_video");
		allTeachers = ret->getBoolean("all_teachers");
		is_student = true;
	}
	return is_student;
}
