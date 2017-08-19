#include "OdinClient.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

OdinClient::OdinClient():
_socket(NULL),
_is_teacher(false),
_all_teachers(false)
{
}

OdinClient::~OdinClient()
{
}

void OdinClient::InitSocketSlot()
{
	if(_socket != NULL)
	{
		connect(_socket, &LokiSocket::OnConnected, this, &OdinClient::OnConnected);
		connect(_socket, &LokiSocket::OnAboutToDelete, this, &OdinClient::OnClientDisconnectTriggered);
		connect(_socket, &LokiSocket::OnDataRead, this, &OdinClient::OnDataReadTriggered);
		connect(_socket, &LokiSocket::OnSocketError, this, &OdinClient::OnSocketError);
	}
}

void OdinClient::Init(QString ip, int port)
{
	if(_socket == NULL)
		_socket = new LokiSocket(NULL);
	InitSocketSlot();
	_socket->ConnectToHost(ip, port);
}

void OdinClient::OnClientDisconnectTriggered(LokiSocket*)
{
	if(_socket != NULL)
	{
		disconnect(_socket, &LokiSocket::OnConnected, this, &OdinClient::OnConnected);
		disconnect(_socket, &LokiSocket::OnAboutToDelete, this, &OdinClient::OnClientDisconnectTriggered);
		disconnect(_socket, &LokiSocket::OnDataRead, this, &OdinClient::OnDataReadTriggered);
		disconnect(_socket, &LokiSocket::OnSocketError, this, &OdinClient::OnSocketError);
		_socket = NULL;
	}

	//这里应该emit一个error, 让retry network
}

void OdinClient::OnDataReadTriggered(LokiBuffer& buffer)
{	
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
		OnCommandTriggered(command);
		delete command;
	}
}

void OdinClient::OnCommandTriggered(Loki::LokiCommand* command)
{
	Loki::LokiCommand::CommandType type = command->GetType();

	if(type == Loki::LokiCommand::LOGIN)
	{
		Loki::LoginCommand* login = static_cast<Loki::LoginCommand*>(command);
		if(login->result)
		{
			_user_id = login->user_id;
			_nickname = QString(login->display_name.c_str());
		}
		Q_EMIT OnLogin(login->result, QString(login->message.c_str()));
	}

	if(type == Loki::LokiCommand::GETFRIENDLIST)
	{
		Loki::GetFriendListCommand* getFriend = static_cast<Loki::GetFriendListCommand*>(command);
		Q_EMIT OnGetFriendList(getFriend->friends);
	}

	if(type == Loki::LokiCommand::CHAT)
	{
		Loki::ChatCommand* chat = static_cast<Loki::ChatCommand*>(command);
		Q_EMIT OnChat(chat->from_id, QString(chat->message.c_str()));
	}

	if(type == Loki::LokiCommand::BORADLOGIN)
	{
		Loki::BoardLoginCommand* login = static_cast<Loki::BoardLoginCommand*>(command);
		if(login->result&&!login->peer)
		{
			_user_id = QString(login->user_id.c_str());
			_nickname = QString(login->nickname.c_str());
			_class_id = QString(login->class_id.c_str());
			_is_teacher = login->is_teacher;
			_all_teachers = login->all_teachers;
		}
		Q_EMIT OnBoardLogin(login->result, login->peer, QString(login->user_id.c_str()), QString(login->nickname.c_str()), login->is_teacher, login->all_teachers);
	}

	if(type == Loki::LokiCommand::HADESDATA)
	{
		Loki::HadesDataCommand* hades = static_cast<Loki::HadesDataCommand*>(command);
		Q_EMIT OnHadesData(QString(hades->from_id.c_str()), QString(hades->nickname.c_str()), hades->data);
	}

	if(type == Loki::LokiCommand::BOARDDATA)
	{
		Loki::BoardDataCommand* board = static_cast<Loki::BoardDataCommand*>(command);
		Q_EMIT OnBoardData(board->data);
	}

	if(type == Loki::LokiCommand::APPLYBOARD)
	{
		Loki::ApplyBoardCommand* apply = static_cast<Loki::ApplyBoardCommand*>(command);
		//这里分出去2种情况
		if(apply->from_teacher)
		{
			if(!_is_teacher)
				Q_EMIT OnTeacherAgreeBoard(apply->student_id != _user_id.toStdString(), apply->agree);
		}
		else
		{
			Q_EMIT OnApplyBoard(QString(apply->student_id.c_str()), QString(apply->nickname.c_str()));
		}
	}

	if(type == Loki::LokiCommand::TAKEBOARD)
	{
		Loki::TakeBoardCommand* take = static_cast<Loki::TakeBoardCommand*>(command);
		Q_EMIT OnTakeBoard();
	}
}

void OdinClient::Disconnect()
{
	_socket->Disconnect();
}

void OdinClient::SendData(int type, LokiBuffer& buffer)
{
	if(_socket == NULL)
		return;

	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);

	packer.pack(type);
	packer.pack_raw(buffer.Size());
	packer.pack_raw_body(buffer.Data(), buffer.Size());

	LokiBuffer packLokiBuffer(sbuffer.data(), sbuffer.size());
	_socket->SendData(packLokiBuffer);
}

void OdinClient::Login(QString& username, QString& password)
{
	_login_name = username;
	Loki::LoginCommand command;
	command.version = TEACH_CLIENT_VERSION;
	command.username = username.toStdString();
	command.password = password.toStdString();
	SendData(command.GetType(), command.GetMsgPackData());
}

void OdinClient::Chat(int from_id, int to_id, QString& message)
{
	Loki::ChatCommand command;
	command.from_id = from_id;
	command.to_id = to_id;
	command.message = message.toStdString();
	SendData(command.GetType(), command.GetMsgPackData());
}

void OdinClient::GetFriendList()
{
	Loki::GetFriendListCommand command;
	SendData(command.GetType(), command.GetMsgPackData());
}

void OdinClient::BoardLogin(QString& session_id)
{
	Loki::BoardLoginCommand command;
	command.session_id = session_id.toStdString();
	SendData(command.GetType(), command.GetMsgPackData());
}

void OdinClient::HadesData(QString peer_id, LokiBuffer& buffer)
{
	Loki::HadesDataCommand command;
	command.class_id = _class_id.toStdString();
	command.nickname = _nickname.toStdString();
	command.from_id = _user_id.toStdString();
	command.to_id = peer_id.toStdString();
	command.data = buffer;
	SendData(command.GetType(), command.GetMsgPackData());
}

void OdinClient::BoardData(LokiBuffer& buffer)
{
	Loki::BoardDataCommand command;
	command.class_id = _class_id.toStdString();
	command.data = buffer;
	SendData(command.GetType(), command.GetMsgPackData());
}

void OdinClient::ApplyBoard()
{
	Loki::ApplyBoardCommand command;
	command.class_id = _class_id.toStdString();
	command.from_teacher = _is_teacher;
	command.student_id = _user_id.toStdString();
	command.nickname = _nickname.toStdString();
	command.agree = false;
	SendData(command.GetType(), command.GetMsgPackData());
}

void OdinClient::AgreeBoard(QString student_id, bool agree)
{
	Loki::ApplyBoardCommand command;
	command.class_id = _class_id.toStdString();
	command.from_teacher = true;
	command.student_id = student_id.toStdString();
	command.agree = agree;
	SendData(command.GetType(), command.GetMsgPackData());
}

void OdinClient::TakeBoard()
{
	Loki::TakeBoardCommand command;
	command.class_id = _class_id.toStdString();
	SendData(command.GetType(), command.GetMsgPackData());
}
