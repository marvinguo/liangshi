#include "TyrCommand.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

namespace Tyr
{

TyrCommand* TyrCommand::CreateFromType(TyrCommandType type, LokiBuffer& buffer)
{
	msgpack::unpacked msg;
	msgpack::unpack(&msg, buffer.Data(), buffer.Size());

	TyrCommand *command = NULL;

	if(type == LS)
	{
		LSCommand* c = new LSCommand();
		msg.get().convert(c);
		command = c;
	}
	else if(type == MV)
	{
		MVCommand* c = new MVCommand();
		msg.get().convert(c);
		command = c;
	}
	else if(type == RM)
	{
		RMCommand* c = new RMCommand();
		msg.get().convert(c);
		command = c;
	}
	else if(type == PUT)
	{
		PUTCommand* c = new PUTCommand();
		msg.get().convert(c);
		command = c;
	}
	else if(type == GET)
	{
		GETCommand* c = new GETCommand();
		msg.get().convert(c);
		command = c;
	}
	else if(type == Zoom)
	{
		ZoomCommand* c = new ZoomCommand();
		msg.get().convert(c);
		command = c;
	}
	else if(type == Pen)
	{
		PenCommand* c = new PenCommand();
		msg.get().convert(c);
		command = c;
	}

	return command;
}

void LSCommand::ls(QString path)
{
	_path = path.toStdString();
}

LokiBuffer LSCommand::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

LokiBuffer MVCommand::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

LokiBuffer RMCommand::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

void PUTCommand::put(QString path, LokiBuffer& buffer)
{
	_path = path.toStdString();
	_content = buffer;
}

LokiBuffer PUTCommand::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

LokiBuffer GETCommand::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

LokiBuffer ZoomCommand::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

LokiBuffer PenCommand::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

}
