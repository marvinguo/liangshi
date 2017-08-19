#include "TyrProtocol.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

TyrProtocol::TyrProtocol(QObject *parent):
QObject(parent),
_client(NULL)
{
}

TyrProtocol::~TyrProtocol()
{
}

void TyrProtocol::SetSocketClient(SocketClient* client)
{
	if(_client != NULL)
	{
		disconnect(_client, SIGNAL(OnDataRead(LokiBuffer&)), this, SLOT(OnDataReadTriggered(LokiBuffer&)));
		disconnect(_client, SIGNAL(OnAboutToDelete(SocketClient*)), this, SLOT(OnClientDisconnectTriggered(SocketClient*)));
	}
	_client = client;
	if(_client != NULL)
	{
		connect(_client, SIGNAL(OnDataRead(LokiBuffer&)), this, SLOT(OnDataReadTriggered(LokiBuffer&)));
		connect(_client, SIGNAL(OnAboutToDelete(SocketClient*)), this, SLOT(OnClientDisconnectTriggered(SocketClient*)));
	}
}

void TyrProtocol::ConnectToHost(QString ip, int port)
{
	_client->ConnectToHost(ip, port);
}

void TyrProtocol::OnClientDisconnectTriggered(SocketClient*)
{
	SetSocketClient(NULL);
}

void TyrProtocol::SendCommand(Tyr::TyrCommand* command)
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);

	packer.pack((int)command->GetType());
	LokiBuffer buffer;
	buffer = command->GetMsgPackData();
	packer.pack_raw(buffer.Size());
	packer.pack_raw_body(buffer.Data(), buffer.Size());

	LokiBuffer packLokiBuffer(sbuffer.data(), sbuffer.size());
	_client->SendData(packLokiBuffer);
}

void TyrProtocol::OnDataReadTriggered(LokiBuffer& buffer)
{
	msgpack::unpacker pac;
	pac.reserve_buffer(buffer.Size());
	memcpy(pac.buffer(), buffer.Data(), buffer.Size());
	pac.buffer_consumed(buffer.Size());

	int type = 0;
	Tyr::TyrCommand* command = NULL;

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
				command = Tyr::TyrCommand::CreateFromType((Tyr::TyrCommandType)type, rawLokiBuffer);
			}
		}
	}

	if(command != NULL)
	{
		Q_EMIT OnCommand(command);
		delete command;
	}
}

void TyrProtocol::ls(QString path)
{
	Tyr::LSCommand* command = new Tyr::LSCommand();
	command->ls(path);
	SendCommand(command);
	delete command;
}

void TyrProtocol::mv(QString oldPath,QString newPath)
{

}

void TyrProtocol::rm(QString path)
{

}

void TyrProtocol::put(QString path, QString localFile)
{
	Tyr::PUTCommand* command = new Tyr::PUTCommand();

	QFile readFile(localFile);
	readFile.open(QIODevice::ReadOnly);
	QByteArray array = readFile.readAll();
	LokiBuffer buffer(array.data(), array.size());
	readFile.close();

	QFileInfo info(localFile);

	command->put(path + "/" + info.fileName(), buffer);
	SendCommand(command);
	delete command;
}

void TyrProtocol::get(QString path, QString localFile)
{

}

