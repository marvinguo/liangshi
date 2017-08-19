#pragma once

#include "../common/library.h"
#include "../common/stl.h"
#include "User.h"

class Responser
{
public:
	Responser();
	virtual ~Responser();

private:
	void AddUser(User* user);
	User* FindUserByHandle(uv_handle_t* handle);
	void DeleteUserByHandle(uv_handle_t* handle);

public:
	static void OnConnection(uv_stream_t* server_handle, int status);
	static void OnClose(uv_handle_t* handle);
	static void OnRead(uv_stream_t* handle, ssize_t nread, uv_buf_t buf);
	static uv_buf_t OnAlloc(uv_handle_t* handle, size_t suggested_size);

private:
	map<uv_handle_t*, User*> _users;
};

