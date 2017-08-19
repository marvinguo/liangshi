#ifndef HTTP_REQUEST
#define HTTP_REQUEST

#include <QObject>
#include <QHash>
#include <QMetaType>
#include <QUrl>

class QTcpSocket;

class HttpConnection;

typedef QHash<QString, QString> HeaderHash;

class HttpRequest : public QObject
{
    Q_OBJECT

public:
    virtual ~HttpRequest();

    const QString& method() const { return m_method; }
    const QUrl& url() const { return m_url; }
    const QString path() const { return m_url.path(); }
    const QString& httpVersion() const { return m_version; }
    const QString & queryString() const {  return m_queryString; }
    const HeaderHash& headers() const { return m_headers; }
    QString header(const QString &field) { return m_headers[field]; }
    const QString& remoteAddress() const { return m_remoteAddress; }
    quint16 remotePort() const { return m_remotePort; }
    QString localAddress() const;

signals:
    /*
     * This may be emitted zero or more times.
     */
    void data(const QByteArray &);
    void end();

private:
    HttpRequest(HttpConnection *connection, QObject *parent = 0);

    void setMethod(const QString &method) { m_method = method; }
    void setVersion(const QString &version) { m_version = version; }
    void setUrl(const QUrl &url) { m_url = url; }
    void setHeaders(const HeaderHash headers) { m_headers = headers; }
    void setHeader(const QString &field, const QString &value){m_headers[field]=value;}
    void setQueryString(const QString &query) { m_queryString = query; }

    HttpConnection *m_connection;
    HeaderHash m_headers;
    QString m_method;
    QUrl m_url;
    QString m_version;
    QString m_remoteAddress;
    quint16 m_remotePort;
    QString m_queryString;
    friend class HttpConnection;
};

#endif
