#pragma once

#include "Qt.h"
#include "stl.h"
#include "config.h"
#include "../../../SharedCode/LokiBuffer.h"

class ThorClient : public QObject
{
	Q_OBJECT
public:
	ThorClient(QObject *parent = 0);
	~ThorClient();

	void SendBroadcastData(LokiBuffer& buffer);

private:
	QUdpSocket *_udp;
};

