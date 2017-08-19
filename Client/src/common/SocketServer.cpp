#include "SocketServer.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

SocketServer::SocketServer(QObject *parent):
QObject(parent),
_maxClient(1),
_tcp(new QTcpServer(this))
{
	connect(_tcp, SIGNAL(newConnection()), this, SLOT(NewConnectionTriggered()));
}

SocketServer::~SocketServer()
{
}

void SocketServer::StartListen()
{
	_tcp->listen();
}

void SocketServer::StopListen()
{
	_tcp->close();
}

void SocketServer::NewConnectionTriggered()
{
	QTcpSocket *socket = _tcp->nextPendingConnection();
	if(_clients.size() >= _maxClient)
	{
		socket->disconnectFromHost();
		return;
	}
	SocketClient* client = new SocketClient(socket);
	connect(client, SIGNAL(OnAboutToDelete(SocketClient*)), this, SLOT(OnClientAboutToDeleteTriggered(SocketClient*)));
	_clients.push_back(client);
	Q_EMIT OnNewClient(client);
}

void SocketServer::OnClientAboutToDeleteTriggered(SocketClient* obj)
{
	disconnect(obj, SIGNAL(OnAboutToDelete(SocketClient*)), this, SLOT(OnClientAboutToDeleteTriggered(SocketClient*)));
	for(vector<SocketClient*>::iterator i = _clients.begin(); i != _clients.end(); ++i)
	{
		if(obj == *i)
		{
			_clients.erase(i);
			break;
		}
	}
}

QString SocketServer::GetLocalNetIp()
{
	QString ipAddress;
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	// use the first non-localhost IPv4 address
	for (int i = 0; i < ipAddressesList.size(); ++i)
	{
		if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
			ipAddressesList.at(i).toIPv4Address())
		{
			ipAddress = ipAddressesList.at(i).toString();
			if(ipAddress.contains("169.254"))
			{
				continue;
			}
			break;
		}
	}

	// if we did not find one, use IPv4 localhost
	if (ipAddress.isEmpty())
		ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

	//return ipAddress;
	return "0.0.0.0";
}