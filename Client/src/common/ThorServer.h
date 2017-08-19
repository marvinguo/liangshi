#pragma once

#include "Qt.h"
#include "stl.h"
#include "config.h"
#include "../../../SharedCode/LokiBuffer.h"

class ThorServer : public QObject
{
	Q_OBJECT
public:
	ThorServer(QObject *parent = 0);
	~ThorServer();

	void StartListen();
	void StopListen();

Q_SIGNALS:
	void OnDataReady(LokiBuffer& buffer);

private Q_SLOTS:
	void OnDataReadTriggered();

private:
	QUdpSocket *_udp;
};

