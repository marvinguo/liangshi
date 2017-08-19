#include "ThorServer.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
	#pragma execution_character_set("utf-8")
#endif

ThorServer::ThorServer(QObject *parent):
QObject(parent),
_udp(new QUdpSocket(this))
{
	connect(_udp, SIGNAL(readyRead()), this, SLOT(OnDataReadTriggered()));
}

ThorServer::~ThorServer()
{
}

void ThorServer::StartListen()
{
	_udp->bind(DEFAULT_BROADCAST_PORT, QUdpSocket::ShareAddress);
}

void ThorServer::StopListen()
{
	_udp->close();
}

void ThorServer::OnDataReadTriggered()
{
	while (_udp->hasPendingDatagrams())
	{
		QByteArray datagram;
		datagram.resize(_udp->pendingDatagramSize());
		_udp->readDatagram(datagram.data(), datagram.size());
		LokiBuffer buffer(datagram.data(), datagram.size());
		Q_EMIT OnDataReady(buffer);
	}
}