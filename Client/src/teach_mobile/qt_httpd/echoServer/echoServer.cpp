#include "echoServer.h"
#include <QTcpSocket>

EchoServer::EchoServer(QObject *parent)
    : QTcpServer(parent)
{
    connect(this, SIGNAL(newConnection()),
            this, SLOT(onNewConnection()));
}

EchoServer::~EchoServer()
{}

void EchoServer::onNewConnection()
{
    while(hasPendingConnections())
    {
        QTcpSocket *sock = nextPendingConnection();
        connect(sock, SIGNAL(readyRead()),
                this, SLOT(onReadyRead()));
        connect(sock, SIGNAL(disconnected()),
                sock, SLOT(deleteLater()));
        if(sock->bytesAvailable() > 0)
        {
            echo(sock);
        }
    }
}

void EchoServer::onReadyRead()
{
    QTcpSocket *sock = qobject_cast<QTcpSocket*>(sender());
    echo(sock);
}

void EchoServer::echo(QTcpSocket *sock)
{
    QByteArray inData = sock->readAll();
    qDebug() << inData;
    sock->write(inData);
    sock->flush();
}
