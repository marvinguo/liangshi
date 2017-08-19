#include "User.h"
#include "NetCommandHandler.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

User::User():
//_uvHandle(NULL)
_blockSize(0)
{

}

User::~User()
{

}

void User::Init()
{
	uv_tcp_init(uv_default_loop(), &_uvTcpHandle);
	//keep alive
	uv_tcp_keepalive(&_uvTcpHandle, 1, 300);
	NetCommandHandler& handler = NetCommandHandler::GetInstance();
	SignalOnReceiveCommand.connect(&handler, &NetCommandHandler::OnReceiveCommand);
}

uv_tcp_t* User::GetUVTcpHandle()
{
	return &_uvTcpHandle;
}

//主动关闭连接？
//恶意的连接
//第二次登录等，需要主动关闭
void User::Close()
{
	 uv_close((uv_handle_t*)GetUVTcpHandle(), NULL);
}

void User::OnData(ssize_t nread, const uv_buf_t* buf)
{
	_received.InsertData(buf->base, nread);

	bool parse = ParseCommand();

	while(parse)
	{
		parse = ParseCommand();
	}
}

bool User::ParseCommand()
{
	if(_blockSize == 0)
	{
		if(_received.Size() >= 4)
		{
			uint32_t nethead = _received.Get4BytesHead();
			_blockSize = ntohl(nethead);
		}
	}

	if(_received.Size() >= _blockSize)
	{
		LokiBuffer buffer = _received.GetBuffer(_blockSize);
		_blockSize = 0;

		msgpack::unpacker pac;
		pac.reserve_buffer(buffer.Size());
		memcpy(pac.buffer(), buffer.Data(), buffer.Size());
		pac.buffer_consumed(buffer.Size());

		int type = 0;
		Loki::LokiCommand* command = NULL;

		msgpack::unpacked result;
		while(pac.next(&result))
		{
			msgpack::object object = result.get();

			if(object.type == msgpack::type::POSITIVE_INTEGER)
			{
				object.convert(&type);
			}
			if(object.type == msgpack::type::RAW)
			{
				if(type != 0)
				{
					LokiBuffer rawLokiBuffer(const_cast<char*>(object.via.raw.ptr), object.via.raw.size);
					command = Loki::LokiCommand::CreateFromType((Loki::LokiCommand::CommandType)type, rawLokiBuffer);
				}
			}
		}

		if(command != NULL)
		{
			command->SetUser(this);
			SignalOnReceiveCommand(command);

			//不能删除，thread里删除
			//delete command;
		}
	}

	return _received.Size() > 4 && _blockSize == 0;
}