#include "ThorProtocol.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
	#pragma execution_character_set("utf-8")
#endif

ThorProtocol::ThorProtocol(ThorClient* client, ThorServer* server, QObject *parent):
QObject(parent),
_client(client),
_server(server)
{
	if(_server != NULL)
	{
		connect(_server, SIGNAL(OnDataReady(LokiBuffer&)), this, SLOT(OnDataReadTriggered(LokiBuffer&)));
	}
}

ThorProtocol::~ThorProtocol()
{
}

void ThorProtocol::SendServerInfo(ThorProtocolType type, QString ip, int port)
{
	_type = type;
	_ip = ip.toStdString();
	_port = port;

	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	_client->SendBroadcastData(buffer);
}

void ThorProtocol::StartListen()
{
	_server->StartListen();
}

void ThorProtocol::StopListen()
{
	_server->StopListen();
}

void ThorProtocol::OnDataReadTriggered(LokiBuffer& buffer)
{
	msgpack::unpacked msg;
	msgpack::unpack(&msg, buffer.Data(), buffer.Size());
	msg.get().convert(this);
	Q_EMIT OnGetServerInfo((ThorProtocolType)_type, QString(_ip.c_str()), _port);
}
