#ifndef Q_HTTP_RESPONSE
#define Q_HTTP_RESPONSE

#include <QObject>
#include <QHash>

//
class QTcpSocket;

class HttpConnection;

typedef QHash<QString, QString> HeaderHash;

class HttpResponse : public QObject
{
    Q_OBJECT

public:
    virtual ~HttpResponse();

    qint64 bytesToWrite();
    void setReadBufferSize(qint64 size);
    qint64 readBufferSize();

    void writeHead(int status);

    /*
     * writeHead() has to be called before write()
     */
    void write(const QByteArray &data);

    /*
     * see write(const QByteArray &data)
     */
    qint64 write(const char * data, qint64 len);

    /*
     * see write(const QByteArray &);
     */
    void write(const QString &data);

    bool flush();

    /*
     * end the response with data, the object will be deleted later
     */
    void end(const QString &data);

    void setHeader(const QString &field, const QString &value);

    void setSocketSendBufferSize(int size);
    int getSocketSendBufferSize();

    quint16 localPort();
    quint16 peerPort();
    QString peerAddress();

public slots:
    void end();

signals:
    void done();

private:
    HttpResponse(HttpConnection *connection);

    void writeHeaders();

private:
    HttpConnection *m_connection;

    bool m_headerWritten;
    HeaderHash m_headers;
    friend class HttpConnection;
};

#endif
