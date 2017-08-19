#pragma once

#include "Qt.h"
#include "stl.h"
#include "library.h"
#include "../../../SharedCode/LokiBuffer.h"

//SocketClient lift depends on socket, no others manage its life, important!!!
//还有，项目中，只用SocketClient, 不用QTcpSocket, important!!!
class LokiSocket : public QObject
{
	Q_OBJECT
public:
	LokiSocket(QTcpSocket* socket);
	virtual ~LokiSocket();

	void ConnectToHost(QString ip, int port);
	void Disconnect();
	void SendData(LokiBuffer& buffer);

Q_SIGNALS:
	void OnDataRead(LokiBuffer& buffer);
	void OnAboutToDelete(LokiSocket* obj);
	void OnConnected();
	void OnSocketError(QAbstractSocket::SocketError socketError);

private Q_SLOTS:
	void OnClientDisconnectTriggered();
	void OnDataReadTriggered();
	void OnSocketErrorTriggered(QAbstractSocket::SocketError socketError);

private:
	void InitSocketSlot();
	bool ParseCommand(QDataStream& in);

private:
	QTcpSocket *_socket;
	//从fortuneclient里偷过来的
	quint32 _blockSize;
};


