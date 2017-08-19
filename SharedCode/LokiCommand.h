#pragma once

#include "stl.h"
#include "LokiBuffer.h"

class User;

namespace Loki
{

	class LokiCommand
	{
	public:
		enum CommandType
		{
			LOGIN = 23311,
			CHAT = 23312,
			GETFRIENDLIST = 23313,
			BORADLOGIN = 23314,
			HADESDATA = 23315,
			BOARDDATA = 23316,
			APPLYBOARD = 23317,
			TAKEBOARD = 23318,
			SERVERNOTIFYRELOGIN = 23319,
			SERVERNOTIFYLOSTCONNECTION = 23320,
		};

		LokiCommand(CommandType type):_type(type), _user(NULL){}
		virtual ~LokiCommand(){}

		void SetUser(User* user){_user = user;}
		User* GetUser(){return _user;}

		CommandType GetType(){return(CommandType) _type;}
		static LokiCommand* CreateFromType(CommandType type, LokiBuffer& buffer);
		virtual LokiBuffer GetMsgPackData() = 0;

	private:
		int _type;
		User* _user;
	};

	class LoginCommand : public LokiCommand
	{
	public:
		LoginCommand():LokiCommand(LOGIN){}
		~LoginCommand(){}
		virtual LokiBuffer GetMsgPackData();

		int user_id;
		int version;
		string username;
		string password;
		string display_name;
		string message;
		bool result;
		MSGPACK_DEFINE(user_id, version, username, password, display_name, message, result);
	};

	class ChatCommand : public LokiCommand
	{
	public:
		ChatCommand():LokiCommand(CHAT){}
		~ChatCommand(){}
		virtual LokiBuffer GetMsgPackData();

		int from_id;
		int to_id;
		string message;
		MSGPACK_DEFINE(from_id, to_id, message);
	};

	enum LokiFriendStatus
	{
		Offline = 1001,
		OnLine = 1002,
	};

	struct LokiFriend
	{
		int user_id;
		int status;
		string display_name;
		string comment_name;
		string group_name;
		MSGPACK_DEFINE(user_id, status, display_name, comment_name, group_name);
	};

	class GetFriendListCommand : public LokiCommand
	{
	public:
		GetFriendListCommand():LokiCommand(GETFRIENDLIST){}
		~GetFriendListCommand(){}
		virtual LokiBuffer GetMsgPackData();

		vector<LokiFriend> friends;
		MSGPACK_DEFINE(friends);
	};

	class BoardLoginCommand : public LokiCommand
	{
	public:
		BoardLoginCommand():LokiCommand(BORADLOGIN){}
		~BoardLoginCommand(){}
		virtual LokiBuffer GetMsgPackData();

		string session_id;
		bool result;
		bool peer;
		string user_id;
		bool is_teacher;
		bool has_video;
		bool all_teachers;
		string nickname;
		string class_id;
		MSGPACK_DEFINE(session_id, result, peer, user_id, is_teacher, has_video, all_teachers, nickname, class_id);
	};

	class HadesDataCommand : public LokiCommand
	{
	public:
		HadesDataCommand():LokiCommand(HADESDATA){}
		~HadesDataCommand(){}
		virtual LokiBuffer GetMsgPackData();

		string class_id;
		string nickname;
		string from_id;
		string to_id;
		LokiBuffer data;
		MSGPACK_DEFINE(class_id, nickname, from_id, to_id, data);
	};

	class BoardDataCommand : public LokiCommand
	{
	public:
		BoardDataCommand():LokiCommand(BOARDDATA){}
		~BoardDataCommand(){}
		virtual LokiBuffer GetMsgPackData();

		string class_id;
		LokiBuffer data;
		MSGPACK_DEFINE(class_id, data);
	};

	class ApplyBoardCommand : public LokiCommand
	{
	public:
		ApplyBoardCommand():LokiCommand(APPLYBOARD){}
		~ApplyBoardCommand(){}
		virtual LokiBuffer GetMsgPackData();

		string class_id;
		string student_id;
		bool from_teacher;
		bool agree;
		string nickname;
		MSGPACK_DEFINE(class_id, student_id, from_teacher, agree, nickname);
	};

	class TakeBoardCommand : public LokiCommand
	{
	public:
		TakeBoardCommand():LokiCommand(TAKEBOARD){}
		~TakeBoardCommand(){}
		virtual LokiBuffer GetMsgPackData();

		string class_id;
		MSGPACK_DEFINE(class_id);
	};

	class ServerNotifyLostRelogin : public LokiCommand
	{
	public:
		ServerNotifyLostRelogin():LokiCommand(SERVERNOTIFYRELOGIN){}
		~ServerNotifyLostRelogin(){}
		virtual LokiBuffer GetMsgPackData();

		int user_id;
		string display_name;
		MSGPACK_DEFINE(user_id, display_name);
	};

	class ServerNotifyLostConnectionCommand : public LokiCommand
	{
	public:
		ServerNotifyLostConnectionCommand():LokiCommand(SERVERNOTIFYLOSTCONNECTION){}
		~ServerNotifyLostConnectionCommand(){}
		virtual LokiBuffer GetMsgPackData();

		int user_id;
		string display_name;
		MSGPACK_DEFINE(user_id, display_name);
	};
}
