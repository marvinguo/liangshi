#ifndef ECHOSERVER_H
#define ECHOSERVER_H
#include <QTcpServer>

class EchoServer : public QTcpServer
{
    Q_OBJECT
public:
    EchoServer(QObject *parent = 0);
    ~EchoServer();

protected slots:
    void onNewConnection();
    void onReadyRead();

private:
    void echo(QTcpSocket *sock);
};

#endif // ECHOSERVER_H
