#include "SocketManager.h"
#include "ClassSessionManager.h"
#include "Console.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

SocketManager& SocketManager::GetInstance()
{
	static SocketManager manager;
	return manager;
}

SocketManager::SocketManager()
{
}

SocketManager::~SocketManager()
{
}

void SocketManager::AddUser(User* user)
{
	_users.insert(pair<uv_handle_t*, User*>((uv_handle_t*)user->GetUVTcpHandle(), user));
}

void SocketManager::AddUserID(User* user)
{
	_IDusers.insert(pair<string, User*>(user->GetUserID(), user));
}

User* SocketManager::FindUserByHandle(uv_handle_t* handle)
{
	map<uv_handle_t*, User*>::iterator it = _users.find(handle);
	if(it != _users.end())
		return it->second;
	return NULL;
}

void SocketManager::DeleteUserByHandle(uv_handle_t* handle)
{
	User* user = FindUserByHandle(handle);
	if(user != NULL)
	{
		ClassSessionManager& sessionMgr = ClassSessionManager::GetInstance();
		sessionMgr.RemoveUser(user);
		delete user;
	}
	_users.erase(handle);
}

User* SocketManager::FindUserByID(string user_id)
{
	map<string, User*>::iterator it = _IDusers.find(user_id);
	if(it != _IDusers.end())
		return it->second;
	return NULL;
}

void SocketManager::OnConnection(uv_stream_t* server_handle, int status)
{
	Console::CheckStatus(status, "OnConnection error");

	User* user = new User();
	user->Init();

	int r = uv_accept(server_handle, (uv_stream_t*)user->GetUVTcpHandle());
	Console::CheckStatus(r, "uv_accept");
	
	// start accepting messages from the user
	uv_read_start((uv_stream_t*)user->GetUVTcpHandle(), SocketManager::OnAlloc, SocketManager::OnRead);
	SocketManager::GetInstance().AddUser(user);
}

void SocketManager::OnClose(uv_handle_t* handle)
{
	SocketManager::GetInstance().DeleteUserByHandle(handle);
}

void SocketManager::OnRead(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf)
{
	// user disconnected
	if (nread < 0)
	{
		if (nread != UV_EOF)
			fprintf(stderr, "Read error %s\n", uv_err_name(nread));
		uv_close((uv_handle_t*)handle, SocketManager::OnClose);
		return;
	}

	//from handle to find user
	User* user = SocketManager::GetInstance().FindUserByHandle((uv_handle_t*)handle);
	if(user != NULL)
	{
		user->OnData(nread, buf);
	}
	free(buf->base);
	//here is the logic
}

void SocketManager::OnAlloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t *buf)
{
	//*buf = uv_buf_init((char*) malloc(suggested_size), suggested_size);
	//uv处处体现着高效，这里的内存在堆栈上分配

	// Return a buffer that wraps a static buffer.
	// Safe because our on_read() allocations never overlap.
	//	static char buf[512];
	//return uv_buf_init(buf, sizeof(buf));
	
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

//做heartbeat
//UV_HANDLE_TCP_KEEPALIVE
void SocketManager::OnTimer(uv_timer_t *handle)
{

}
