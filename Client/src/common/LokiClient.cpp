#include "LokiClient.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

LokiClient::LokiClient():
_socket(NULL),
_is_teacher(false),
_all_teachers(false)
{
}

LokiClient::~LokiClient()
{
	if(_socket != NULL)
	{
		disconnect(_socket, &LokiSocket::OnConnected, this, &LokiClient::OnConnected);
		disconnect(_socket, &LokiSocket::OnAboutToDelete, this, &LokiClient::OnClientDisconnectTriggered);
		disconnect(_socket, &LokiSocket::OnDataRead, this, &LokiClient::OnDataReadTriggered);
		disconnect(_socket, &LokiSocket::OnSocketError, this, &LokiClient::OnSocketError);
		_socket->Disconnect();
		_socket->deleteLater();
		//delete _socket;
		_socket = NULL;
	}
}

void LokiClient::InitSocketSlot()
{
	if(_socket != NULL)
	{
		connect(_socket, &LokiSocket::OnConnected, this, &LokiClient::OnConnected);
		connect(_socket, &LokiSocket::OnAboutToDelete, this, &LokiClient::OnClientDisconnectTriggered);
		connect(_socket, &LokiSocket::OnDataRead, this, &LokiClient::OnDataReadTriggered);
		connect(_socket, &LokiSocket::OnSocketError, this, &LokiClient::OnSocketError);
	}
}

void LokiClient::Init(QString ip, int port)
{
	if(_socket == NULL)
		_socket = new LokiSocket(NULL);
	InitSocketSlot();
	_socket->ConnectToHost(ip, port);
}

void LokiClient::OnClientDisconnectTriggered(LokiSocket*)
{
	if(_socket != NULL)
	{
		disconnect(_socket, &LokiSocket::OnConnected, this, &LokiClient::OnConnected);
		disconnect(_socket, &LokiSocket::OnAboutToDelete, this, &LokiClient::OnClientDisconnectTriggered);
		disconnect(_socket, &LokiSocket::OnDataRead, this, &LokiClient::OnDataReadTriggered);
		disconnect(_socket, &LokiSocket::OnSocketError, this, &LokiClient::OnSocketError);
	}

	//这里应该emit一个error, 让retry network
	//Q_EMIT OnSocketError(QAbstractSocket::RemoteHostClosedError);
}

void LokiClient::OnDataReadTriggered(LokiBuffer& buffer)
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

void LokiClient::OnCommandTriggered(Loki::LokiCommand* command)
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
		QString message(login->message.c_str());
		Q_EMIT OnLogin(login->result, message);
	}

	if(type == Loki::LokiCommand::GETFRIENDLIST)
	{
		Loki::GetFriendListCommand* getFriend = static_cast<Loki::GetFriendListCommand*>(command);
		Q_EMIT OnGetFriendList(getFriend->friends);
	}

	if(type == Loki::LokiCommand::CHAT)
	{
		Loki::ChatCommand* chat = static_cast<Loki::ChatCommand*>(command);
		QString message(chat->message.c_str());
		Q_EMIT OnChat(chat->from_id, message);
	}

	if(type == Loki::LokiCommand::BORADLOGIN)
	{
		Loki::BoardLoginCommand* login = static_cast<Loki::BoardLoginCommand*>(command);
		if(login->result&&!login->peer)
		{
			_user_id = QString(login->user_id.c_str());
			_nickname = QString(login->nickname.c_str());
			_class_id = QString(login->class_id.c_str());
			_has_video = login->has_video;
			_is_teacher = login->is_teacher;
			_all_teachers = login->all_teachers;
		}
		Q_EMIT OnBoardLogin(*login);
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

void LokiClient::Disconnect()
{
	_socket->Disconnect();
}

void LokiClient::SendData(int type, LokiBuffer& buffer)
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

void LokiClient::Login(QString& username, QString& password)
{
	_login_name = username;
	Loki::LoginCommand command;
	command.version = TEACH_CLIENT_VERSION;
	command.username = username.toStdString();
	command.password = password.toStdString();
	LokiBuffer data = command.GetMsgPackData();
	SendData(command.GetType(), data);
}

void LokiClient::Chat(int from_id, int to_id, QString& message)
{
	Loki::ChatCommand command;
	command.from_id = from_id;
	command.to_id = to_id;
	command.message = message.toStdString();
	LokiBuffer data = command.GetMsgPackData();
	SendData(command.GetType(), data);
}

void LokiClient::GetFriendList()
{
	Loki::GetFriendListCommand command;
	LokiBuffer data = command.GetMsgPackData();
	SendData(command.GetType(), data);
}

void LokiClient::BoardLogin(QString& session_id)
{
	Loki::BoardLoginCommand command;
	command.session_id = session_id.toStdString();
	LokiBuffer data = command.GetMsgPackData();
	SendData(command.GetType(), data);
}

void LokiClient::HadesData(QString peer_id, LokiBuffer& buffer)
{
	Loki::HadesDataCommand command;
	command.class_id = _class_id.toStdString();
	command.nickname = _nickname.toStdString();
	command.from_id = _user_id.toStdString();
	command.to_id = peer_id.toStdString();
	command.data = buffer;
	LokiBuffer data = command.GetMsgPackData();
	SendData(command.GetType(), data);
}

void LokiClient::BoardData(LokiBuffer& buffer)
{
	Loki::BoardDataCommand command;
	command.class_id = _class_id.toStdString();
	command.data = buffer;
	LokiBuffer data = command.GetMsgPackData();
	SendData(command.GetType(), data);
}

void LokiClient::ApplyBoard()
{
	Loki::ApplyBoardCommand command;
	command.class_id = _class_id.toStdString();
	command.from_teacher = _is_teacher;
	command.student_id = _user_id.toStdString();
	command.nickname = _nickname.toStdString();
	command.agree = false;
	LokiBuffer data = command.GetMsgPackData();
	SendData(command.GetType(), data);
}

void LokiClient::AgreeBoard(QString student_id, bool agree)
{
	Loki::ApplyBoardCommand command;
	command.class_id = _class_id.toStdString();
	command.from_teacher = true;
	command.student_id = student_id.toStdString();
	command.agree = agree;
	LokiBuffer data = command.GetMsgPackData();
	SendData(command.GetType(), data);
}

void LokiClient::TakeBoard()
{
	Loki::TakeBoardCommand command;
	command.class_id = _class_id.toStdString();
	LokiBuffer data = command.GetMsgPackData();
	SendData(command.GetType(), data);
}
