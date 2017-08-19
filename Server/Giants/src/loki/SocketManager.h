#pragma once

#include "../common/library.h"
#include "../common/stl.h"
#include "User.h"

//是不是结合libuv, 这个类不需要考虑同步问题呢？
class SocketManager
{
public:
	SocketManager();
	virtual ~SocketManager();
	static SocketManager& GetInstance();
	void AddUserID(User* user);
	User* FindUserByID(string user_id);

private:
	void AddUser(User* user);
	User* FindUserByHandle(uv_handle_t* handle);
	void DeleteUserByHandle(uv_handle_t* handle);

public:
	static void OnConnection(uv_stream_t* server_handle, int status);
	static void OnClose(uv_handle_t* handle);
	static void OnRead(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf);
	static void OnAlloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t *buf);
	static void OnTimer(uv_timer_t *handle);

private:
	//owns
	map<uv_handle_t*, User*> _users;
	//quit_finds
	map<string, User*> _IDusers;
};

