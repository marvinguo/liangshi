#pragma once

#include "../common/library.h"
#include "../common/stl.h"

#include "../../../../SharedCode/LokiReceiveBuffer.h"
#include "../../../../SharedCode/LokiCommand.h"
#include "../../../../SharedCode/sigslot_loki.h"

#include "User.h"

class NetCommandHandler : public sigslot_loki::has_slots<>
{
public:
	NetCommandHandler();
	virtual ~NetCommandHandler();
	static NetCommandHandler& GetInstance();

	//handler thread to work out the logic

public:
	void OnReceiveCommand(Loki::LokiCommand* command);

public:
	static void OnWrite(uv_write_t *req, int status);
	static void HandlThread(void* parameter);

private:
	static void SendCommand(Loki::LokiCommand* command);
	static void WriteData(uv_stream_t* handle, LokiBuffer& buffer);
};

