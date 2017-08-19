#include "LokiSocket.h"

#if defined(Q_OS_WIN32)
	#define WIN32_LEAN_AND_MEAN
	//#include <WinSock2.h>
#else
	#include<sys/types.h>
	#include <sys/socket.h>
#endif


#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

LokiSocket::LokiSocket(QTcpSocket* socket):
_socket(socket),
_blockSize(0)
{
	InitSocketSlot();
}

LokiSocket::~LokiSocket()
{
	if(_socket != NULL)
	{
		//_socket->deleteLater();
		//delete _socket;
		_socket = NULL;
	}
}

void LokiSocket::InitSocketSlot()
{
	if(_socket != NULL)
	{
		connect(_socket, &QTcpSocket::connected, this, &LokiSocket::OnConnected);
		connect(_socket, &QTcpSocket::disconnected, _socket, &QTcpSocket::deleteLater);
		connect(_socket, &QTcpSocket::disconnected, this, &LokiSocket::OnClientDisconnectTriggered);
		connect(_socket, &QTcpSocket::readyRead, this, &LokiSocket::OnDataReadTriggered);
		connect(_socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &LokiSocket::OnSocketErrorTriggered);
	}
}

void LokiSocket::ConnectToHost(QString ip, int port)
{
	if(_socket == NULL)
		_socket = new QTcpSocket();
	InitSocketSlot();
	//_socket->abort();
	_socket->reset();
	_socket->connectToHost(ip, port);

	int maxIdle = 30; /* seconds */
	int enableKeepAlive = 1;
	int fd = _socket->socketDescriptor();
	//这个功能需要吗？
	//setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (const char*)&enableKeepAlive, sizeof(enableKeepAlive));
}

void LokiSocket::Disconnect()
{
	_socket->disconnectFromHost();
}

//http://qt-project.org/forums/viewthread/15578
//You may also be interested in QDataStream [qt-project.org] as it allows easy serialization of data, to format which is independent of system and processor.
void LokiSocket::SendData(LokiBuffer& buffer)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_0);
	out << (quint32)0;
	
	//这个是不工作的
	//out << buffer;
	out.writeRawData(buffer.Data(), buffer.Size());
	
	//跳到头，改写block_size
	out.device()->seek(0);

	quint32 length = (quint32)(block.size() - sizeof(quint32));
	out << length;

	int n = _socket->write(block);
	_socket->flush();
}

void LokiSocket::OnClientDisconnectTriggered()
{
	if(_socket != NULL)
	{
		//_socket->deleteLater();
		//delete _socket;
		if(!_socket->isValid())
		{
			_socket = NULL;
			//这个地方该不该发呢？
			//这东西好像没用了，现在socket里，自己管理_socket的生命吧，我想这样的设计比较好
			//Q_EMIT OnAboutToDelete(this);
		}
	}
}

void LokiSocket::OnSocketErrorTriggered(QAbstractSocket::SocketError socketError)
{
	if(_socket != NULL)
	{
		if(!_socket->isValid())
		{
			//_socket->deleteLater();
			//delete _socket;
			_socket = NULL;
			Q_EMIT OnSocketError(socketError);
		}
	}
}


void LokiSocket::OnDataReadTriggered()
{
	QDataStream in(_socket);
	in.setVersion(QDataStream::Qt_4_0);

	bool parse = ParseCommand(in);
	while(parse)
	{
		parse = ParseCommand(in);
	}
}

bool LokiSocket::ParseCommand(QDataStream& in)
{
	int available = _socket->bytesAvailable();

	if(_blockSize == 0 && available > 4)
	{
		in >> _blockSize;
		available = available - 4;
	}
	if(available < _blockSize)
		return false;

	QByteArray data = _socket->read(_blockSize);
	_blockSize = 0;
	LokiBuffer buffer(data.data(), data.size());
	Q_EMIT OnDataRead(buffer);
	return _socket->bytesAvailable() > 4 && _blockSize == 0;
}
