#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>

class Client : public QObject
{
  Q_OBJECT
public:
    Client():m_socket(0){}
    ~Client(){}

    void startConnect(QString host, quint16 port);

protected slots:
    void onConnected();
    void onReadyRead();

private:
    QTcpSocket *m_socket;
};

#endif // CLIENT_H
