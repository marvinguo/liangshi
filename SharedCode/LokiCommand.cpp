#include "LokiCommand.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

namespace Loki
{
	LokiCommand* LokiCommand::CreateFromType(CommandType type, LokiBuffer& buffer)
	{
		msgpack::unpacked msg;
		msgpack::unpack(&msg, buffer.Data(), buffer.Size());

		LokiCommand *command = NULL;
		try
		{
			if(type == LOGIN)
			{
				LoginCommand* c = new LoginCommand();
				msg.get().convert(c);
				command = c;
			}
			else if(type == CHAT)
			{
				ChatCommand* c = new ChatCommand();
				msg.get().convert(c);
				command = c;
			}
			else if(type == GETFRIENDLIST)
			{
				GetFriendListCommand* c = new GetFriendListCommand();
				msg.get().convert(c);
				command = c;
			}
			else if(type == BORADLOGIN)
			{
				BoardLoginCommand* c = new BoardLoginCommand();
				msg.get().convert(c);
				command = c;
			}
			else if(type == HADESDATA)
			{
				HadesDataCommand* c = new HadesDataCommand();
				msg.get().convert(c);
				command = c;
			}
			else if(type == BOARDDATA)
			{
				BoardDataCommand* c = new BoardDataCommand();
				msg.get().convert(c);
				command = c;
			}
			else if(type == APPLYBOARD)
			{
				ApplyBoardCommand* c = new ApplyBoardCommand();
				msg.get().convert(c);
				command = c;
			}
			else if(type == TAKEBOARD)
			{
				TakeBoardCommand* c = new TakeBoardCommand();
				msg.get().convert(c);
				command = c;
			}
			else if(type == SERVERNOTIFYRELOGIN)
			{
				ServerNotifyLostRelogin* c = new ServerNotifyLostRelogin();
				msg.get().convert(c);
				command = c;
			}
			else if(type == SERVERNOTIFYLOSTCONNECTION)
			{
				ServerNotifyLostConnectionCommand* c = new ServerNotifyLostConnectionCommand();
				msg.get().convert(c);
				command = c;
			}
		}
		catch(...)
		{
			command = NULL;
		}

		return command;
	}

	LokiBuffer LoginCommand::GetMsgPackData()
	{
		msgpack::sbuffer sbuffer;
		msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
		packer.pack(*this);
		LokiBuffer buffer(sbuffer.data(), sbuffer.size());
		return buffer;
	}

	LokiBuffer ChatCommand::GetMsgPackData()
	{
		msgpack::sbuffer sbuffer;
		msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
		packer.pack(*this);
		LokiBuffer buffer(sbuffer.data(), sbuffer.size());
		return buffer;
	}

	LokiBuffer GetFriendListCommand::GetMsgPackData()
	{
		msgpack::sbuffer sbuffer;
		msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
		packer.pack(*this);
		LokiBuffer buffer(sbuffer.data(), sbuffer.size());
		return buffer;
	}

	LokiBuffer BoardLoginCommand::GetMsgPackData()
	{
		msgpack::sbuffer sbuffer;
		msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
		packer.pack(*this);
		LokiBuffer buffer(sbuffer.data(), sbuffer.size());
		return buffer;
	}

	LokiBuffer HadesDataCommand::GetMsgPackData()
	{
		msgpack::sbuffer sbuffer;
		msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
		packer.pack(*this);
		LokiBuffer buffer(sbuffer.data(), sbuffer.size());
		return buffer;
	}

	LokiBuffer BoardDataCommand::GetMsgPackData()
	{
		msgpack::sbuffer sbuffer;
		msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
		packer.pack(*this);
		LokiBuffer buffer(sbuffer.data(), sbuffer.size());
		return buffer;
	}

	LokiBuffer ApplyBoardCommand::GetMsgPackData()
	{
		msgpack::sbuffer sbuffer;
		msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
		packer.pack(*this);
		LokiBuffer buffer(sbuffer.data(), sbuffer.size());
		return buffer;
	}

	LokiBuffer TakeBoardCommand::GetMsgPackData()
	{
		msgpack::sbuffer sbuffer;
		msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
		packer.pack(*this);
		LokiBuffer buffer(sbuffer.data(), sbuffer.size());
		return buffer;
	}

	LokiBuffer ServerNotifyLostRelogin::GetMsgPackData()
	{
		msgpack::sbuffer sbuffer;
		msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
		packer.pack(*this);
		LokiBuffer buffer(sbuffer.data(), sbuffer.size());
		return buffer;
	}

	LokiBuffer ServerNotifyLostConnectionCommand::GetMsgPackData()
	{
		msgpack::sbuffer sbuffer;
		msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
		packer.pack(*this);
		LokiBuffer buffer(sbuffer.data(), sbuffer.size());
		return buffer;
	}
}
