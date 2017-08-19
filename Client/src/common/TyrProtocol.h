#pragma once

#include "Qt.h"
#include "stl.h"
#include "../../../SharedCode/LokiBuffer.h"
#include "SocketClient.h"
#include "TyrCommand.h"

class TyrProtocol : public QObject
{
	Q_OBJECT
public:
	TyrProtocol(QObject *parent = 0);
	~TyrProtocol();

public:
	void SetSocketClient(SocketClient* client);
	void ConnectToHost(QString ip, int port);
	void SendCommand(Tyr::TyrCommand* command);

Q_SIGNALS:
	void OnCommand(Tyr::TyrCommand* command);

private Q_SLOTS:
	void OnDataReadTriggered(LokiBuffer&);
	void OnClientDisconnectTriggered(SocketClient*);

//operations
public:
	void ls(QString path);
	void mv(QString oldPath,QString newPath);
	void rm(QString path);
	void put(QString path, QString localFile);
	void get(QString path, QString localFile);

private:
	SocketClient* _client;
};

