#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

#include <QTcpServer>
#include <QHostAddress>
#include <QString>
#include "serverconfig.h"

class HttpServerPrivate;

class HttpServer : public QTcpServer
{
    Q_OBJECT
public:
    HttpServer(QObject * parent = 0);
    ~HttpServer();

    void work(void *data);
    bool requireEventLoop(){return true;}

protected:
    void incomingConnection(qintptr socketDescriptor);


private:
    HttpServerPrivate * m_d;
};

void startServer();

#endif // HTTPSERVER_H
