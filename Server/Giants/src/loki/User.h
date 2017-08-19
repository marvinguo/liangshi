#pragma once

#include "../common/library.h"
#include "../common/stl.h"

#include "../../../../SharedCode/LokiReceiveBuffer.h"
#include "../../../../SharedCode/LokiCommand.h"
#include "../../../../SharedCode/sigslot_loki.h"

class User
{
public:
	User();
	virtual ~User();

private:
	enum State
	{
		INIT,
		LOGIN,
		LOGOUT,
	};

public:
	void Init();
	uv_tcp_t* GetUVTcpHandle();
	void Close();
	void OnData(ssize_t nread, const uv_buf_t* buf);

	void SetUserID(string& user_id){_user_id = user_id;}
	string GetUserID(){return _user_id;}

public:
	sigslot_loki::signal1<Loki::LokiCommand*> SignalOnReceiveCommand;

public:
	static void OnWrite(uv_write_t *req, int status);

private:
	bool ParseCommand();

private:
	uv_tcp_t _uvTcpHandle;
	LokiReceiveBuffer _received;
	uint32_t _blockSize;

private:
	string _user_id;
	bool _is_teacher;
};

