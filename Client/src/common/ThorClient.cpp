#include "ThorClient.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
	#pragma execution_character_set("utf-8")
#endif

ThorClient::ThorClient(QObject *parent):
QObject(parent),
_udp(new QUdpSocket(this))
{

}

ThorClient::~ThorClient()
{
}

void ThorClient::SendBroadcastData(LokiBuffer& buffer)
{
	_udp->writeDatagram(buffer.Data(), buffer.Size(), QHostAddress::Broadcast, DEFAULT_BROADCAST_PORT);
}