#include "Responser.h"
#include "Console.h"

Responser::Responser()
{

}

Responser::~Responser()
{

}

void Responser::AddUser(User* user)
{
	_users.insert(pair<uv_handle_t*, User*>((uv_handle_t*)user->GetUVTcpHandle(), user));
}

User* Responser::FindUserByHandle(uv_handle_t* handle)
{
	map<uv_handle_t*, User*>::iterator it = _users.find(handle);
	if(it != _users.end())
		return it->second;
	return NULL;
}

void Responser::DeleteUserByHandle(uv_handle_t* handle)
{
	User* user = FindUserByHandle(handle);
	if(user != NULL)
		delete user;
	_users.erase(handle);
}

static Responser theResponser;

void Responser::OnConnection(uv_stream_t* server_handle, int status)
{
	assert(status == 0);

	User* user = new User();
	user->Init();

	if (uv_accept(server_handle, (uv_stream_t*)user->GetUVTcpHandle()))
	{
		Console::Fatal("uv_accept");
	}
	
	// start accepting messages from the user
	uv_read_start((uv_stream_t*)user->GetUVTcpHandle(), Responser::OnAlloc, Responser::OnRead);
	theResponser.AddUser(user);
}

void Responser::OnClose(uv_handle_t* handle)
{
	theResponser.DeleteUserByHandle(handle);
}

void Responser::OnRead(uv_stream_t* handle, ssize_t nread, uv_buf_t buf)
{
	// user disconnected
	if (nread == -1)
	{
		uv_close((uv_handle_t*)handle, Responser::OnClose);
		return;
	}

	//from handle to find user
	User* user = theResponser.FindUserByHandle((uv_handle_t*)handle);
	if(user != NULL)
	{
		user->OnData(nread, buf);
	}
	//here is the logic
}

uv_buf_t Responser::OnAlloc(uv_handle_t* handle, size_t suggested_size)
{
	//uv处处体现着高效，这里的内存在堆栈上分配

	// Return a buffer that wraps a static buffer.
	// Safe because our on_read() allocations never overlap.
	static char buf[512];
	return uv_buf_init(buf, sizeof(buf));
}