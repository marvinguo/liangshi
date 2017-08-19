#pragma once

#include "Qt.h"
#include "stl.h"
#include "SocketClient.h"

//先保证一个连接，然后想别的
class SocketServer : public QObject
{
	Q_OBJECT
public:
	SocketServer(QObject *parent = 0);
	~SocketServer();

	void StartListen();
	void StopListen();

	int GetPort(){return _tcp->serverPort();}
	QString GetLocalNetIp();

Q_SIGNALS:
	void OnNewClient(SocketClient *);

private Q_SLOTS:
	void NewConnectionTriggered();
	void OnClientAboutToDeleteTriggered(SocketClient* obj);

private:
	QTcpServer *_tcp;
	vector<SocketClient*> _clients;
	int _maxClient;
};

