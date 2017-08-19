#include "NetCommandHandler.h"
#include "DatabaseEngine.h"
#include "SocketManager.h"
#include "ClassSessionManager.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

NetCommandHandler& NetCommandHandler::GetInstance()
{
	static NetCommandHandler hander;
	return hander;
}

NetCommandHandler::NetCommandHandler()
{
}

NetCommandHandler::~NetCommandHandler()
{

}

void NetCommandHandler::OnReceiveCommand(Loki::LokiCommand* command)
{
	//多线程
	//uv_thread_t thread_id;
	//uv_thread_create(&thread_id, NetCommandHandler::HandlThread, command);
	
	//主线程
	NetCommandHandler::HandlThread(command);
}

void NetCommandHandler::HandlThread(void* parameter)
{
	Loki::LokiCommand* command = (Loki::LokiCommand*)parameter;
	DatabaseEngine& engine = DatabaseEngine::GetInstance();
	SocketManager& socketMgr = SocketManager::GetInstance();
	ClassSessionManager& sessionMgr = ClassSessionManager::GetInstance();

	Loki::LokiCommand::CommandType type = command->GetType();
	User* fromUser = command->GetUser();

	if(type == Loki::LokiCommand::LOGIN)
	{
		Loki::LoginCommand* login = static_cast<Loki::LoginCommand*>(command);
		/*
		string username = login->username;
		string password = login->password;
		
		//empty login string
		login->username = "";
		login->password = "";
		login->result = engine.CheckUserPassword(username, password, login->user_id, login->display_name);
		User* user = command->GetUser();
//		user->SetUserID(login->user_id);
		socketMgr.AddUserID(user);
		NetCommandHandler::SendCommand(command);
		*/
	}

	if(type == Loki::LokiCommand::CHAT)
	{
		Loki::ChatCommand* chat = static_cast<Loki::ChatCommand*>(command);
		/*
		User* user = socketMgr.FindUserByID(chat->to_id);

		//如果空，说明已经下线,还有，那个协作问问题，可以离线
		//从对方的socket发出
		if(user != NULL)
		{
			command->SetUser(user);
		}
		NetCommandHandler::SendCommand(command);
		*/
	}

	if(type == Loki::LokiCommand::GETFRIENDLIST)
	{
		Loki::GetFriendListCommand* friendList = static_cast<Loki::GetFriendListCommand*>(command);
/*
		int user_id = command->GetUser()->GetUserID();
		vector<Loki::LokiFriend> friends;
		engine.GetUserFriendList(user_id, friends);
		friendList->friends = friends;

		NetCommandHandler::SendCommand(command);
*/
	}

	if(type == Loki::LokiCommand::BORADLOGIN)
	{
		Loki::BoardLoginCommand* login = static_cast<Loki::BoardLoginCommand*>(command);

		string class_id;
		string user_id;
		string nickname;
		bool hasVideo = false;
		bool is_teacher = engine.IsTeacher(login->session_id, class_id, user_id, nickname, hasVideo);
		bool is_student = false;
		bool allTeachers = false;
		//如果是live demo, 都是teacher

		if(!is_teacher)
		{
			is_student = engine.IsStudent(login->session_id, class_id, user_id, nickname, hasVideo, allTeachers);
		}

		if(!is_teacher && !is_student)
		{
			login->result = false;
			login->peer = false;
			NetCommandHandler::SendCommand(command);
			delete command;
			return;
		}

		//2014.0521,想起来一个问题,检查是否已经重复登录
		//1.在所有课堂里检查
		//2.第二个登录，替换第一个，server需要发消息

		fromUser->SetUserID(user_id);
		//自己登录正确
		sessionMgr.AddClassUser(class_id, fromUser, is_teacher);
		set<User*> users = sessionMgr.GetClassUsers(class_id);

		login->result = true;
		login->peer = false;
		login->is_teacher = is_teacher;
		login->has_video = hasVideo;
		login->all_teachers = allTeachers;
		login->user_id = user_id;
		login->nickname = nickname;
		login->class_id = class_id;
		NetCommandHandler::SendCommand(command);

		//通知其它用户已经登录
		for(set<User*>::iterator it = users.begin(); it != users.end(); ++it)
		{
			User* user = *it;
			if(user == fromUser)
				continue;
			command->SetUser(user);
			login->peer = true;
			login->result = true;
			NetCommandHandler::SendCommand(command);
			continue;
		}

		//已经登录的人，通知自己
		//这个逻辑不需要
		/*
		for(set<User*>::iterator it = users.begin(); it != users.end(); ++it)
		{
			User* user = *it;
			if(user == fromUser)
				continue;
			command->SetUser(fromUser);
			login->message = "peer user logined";
			login->peer = true;
			login->result = true;
			NetCommandHandler::SendCommand(command);
			continue;
		}
		*/
	}

	if(type == Loki::LokiCommand::HADESDATA)
	{
		Loki::HadesDataCommand* hades = static_cast<Loki::HadesDataCommand*>(command);
		
		set<User*> users = sessionMgr.GetClassUsers(hades->class_id);
		for(set<User*>::iterator it = users.begin(); it != users.end(); ++it)
		{
			User* user = *it;
			if(user->GetUserID() != hades->to_id)
				continue;
			command->SetUser(user);
			NetCommandHandler::SendCommand(command);
		}
	}

	if(type == Loki::LokiCommand::BOARDDATA)
	{
		Loki::BoardDataCommand* data = static_cast<Loki::BoardDataCommand*>(command);

		set<User*> users = sessionMgr.GetClassUsers(data->class_id);
		for(set<User*>::iterator it = users.begin(); it != users.end(); ++it)
		{
			User* user = *it;
			if(user == fromUser)
				continue;
			command->SetUser(user);
			NetCommandHandler::SendCommand(command);
		}
	}

	if(type == Loki::LokiCommand::APPLYBOARD)
	{
		Loki::ApplyBoardCommand* apply = static_cast<Loki::ApplyBoardCommand*>(command);

		if(apply->from_teacher)
		{
			set<User*> users = sessionMgr.GetClassUsers(apply->class_id);
			for(set<User*>::iterator it = users.begin(); it != users.end(); ++it)
			{
				//给所有学生发
				User* user = *it;
				command->SetUser(user);
				NetCommandHandler::SendCommand(command);
			}
		}
		else
		{
			User* teacher = sessionMgr.GetClassTeacher(apply->class_id);
			if(teacher != NULL)
			{
				command->SetUser(teacher);
				NetCommandHandler::SendCommand(command);
			}
		}
	}

	if(type == Loki::LokiCommand::TAKEBOARD)
	{
		Loki::TakeBoardCommand* take = static_cast<Loki::TakeBoardCommand*>(command);

		//只用老师能发，不能自己发送
		set<User*> users = sessionMgr.GetClassUsers(take->class_id);
		for(set<User*>::iterator it = users.begin(); it != users.end(); ++it)
		{
			User* user = *it;
			if(user == fromUser)
				continue;
			command->SetUser(user);
			NetCommandHandler::SendCommand(command);
		}
	}

	if(type == Loki::LokiCommand::SERVERNOTIFYLOSTCONNECTION)
	{
		Loki::ServerNotifyLostConnectionCommand* lost = static_cast<Loki::ServerNotifyLostConnectionCommand*>(command);
	}

	delete command;
}


void NetCommandHandler::SendCommand(Loki::LokiCommand* command)
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);

	int type = (int)command->GetType();
	LokiBuffer buffer = command->GetMsgPackData();
	packer.pack(type);
	packer.pack_raw(buffer.Size());
	packer.pack_raw_body(buffer.Data(), buffer.Size());

	LokiBuffer packLokiBuffer(sbuffer.data(), sbuffer.size());
	uv_tcp_t* handle = command->GetUser()->GetUVTcpHandle();
	NetCommandHandler::WriteData((uv_stream_t*)handle, packLokiBuffer);
}

void NetCommandHandler::WriteData(uv_stream_t* handle, LokiBuffer& buffer)
{
	//UV_EXTERN int uv_is_readable(const uv_stream_t* handle);
	//UV_EXTERN int uv_is_writable(const uv_stream_t* handle);
	//不能writable的hanble,啥时候收到OnClose
	if(!uv_is_writable(handle))
		return;

	if(!uv_is_readable(handle))
		return;

	if(uv_is_closing((uv_handle_t*)handle))
		return;

	if(handle->type == UV_UNKNOWN_HANDLE)
		return;
	//逻辑在这里吗？可能需要在这里处理 OnWrite
	//buffer 在堆栈上，以后可能考虑堆，我想在c++11中支持
	int size = buffer.Size();

	//需要加4个字节头
	LokiReceiveBuffer with4Head;
	with4Head.InsertNetworkUInt32(size);
	with4Head.InsertData(buffer.Data(), buffer.Size());
	LokiBuffer allBuffer = with4Head.GetBuffer(with4Head.Size());

	int len = allBuffer.Size();
	uv_write_t* req = (uv_write_t *)malloc(sizeof(uv_write_t) + len);
	void* addr = req + 1;
	memcpy(addr, allBuffer.Data(), len);
	uv_buf_t buf = uv_buf_init((char*)addr, len);
	uv_write(req, (uv_stream_t*)handle, &buf, 1, NetCommandHandler::OnWrite);
}

void NetCommandHandler::OnWrite(uv_write_t *req, int status)
{
	free(req);
	//char* buffer = (char*)(req->data);
	//delete []buffer;
	/*
	if (status == -1) {
		fprintf(stderr, "Write error %s\n", uv_err_name(uv_last_error(loop)));
	}
	char *base = (char*) req->data;
	free(base);
	free(req);
	*/
}


