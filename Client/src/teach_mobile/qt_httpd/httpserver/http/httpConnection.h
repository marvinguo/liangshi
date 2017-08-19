#ifndef HTTP_CONNECTION
#define HTTP_CONNECTION

#include <QObject>
#include <QHash>
#include <QTcpSocket>

class HttpRequest;
class HttpResponse;

typedef QHash<QString, QString> HeaderHash;

class HttpConnection : public QTcpSocket
{
    Q_OBJECT

public:
    HttpConnection(QObject *parent = 0);
    virtual ~HttpConnection();

    void end();
    void setSocketSendBufferSize(int size);
    int getSocketSendBufferSize();

signals:
    void newRequest(HttpRequest*, HttpResponse*);

private slots:
    void parseRequest();
    void onError(QAbstractSocket::SocketError socketError);

private:
    // since there can only be one request at any time
    // even with pipelining
    HttpRequest *m_request;
    HttpResponse *m_response;

    QString m_reqHeader;
};

#endif
