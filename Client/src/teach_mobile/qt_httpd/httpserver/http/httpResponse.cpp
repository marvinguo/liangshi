#include "httpResponse.h"
#include "httpConnection.h"
#include <QHostAddress>
#include "asyncLog.h"
extern QHash<int, QString> STATUS_CODES;

HttpResponse::HttpResponse(HttpConnection *connection)
    : QObject(0)
    , m_connection(connection)
    , m_headerWritten(false)
{
    //added by anxiaohui , 20111115
    connect(m_connection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(end()));
}

HttpResponse::~HttpResponse()
{
    aDebug() << "~HttpResponse" << endl;
    if(m_connection)
    {
        m_connection->deleteLater();
        m_connection = 0;
    }
}

qint64 HttpResponse::bytesToWrite()
{
    //aDebug() << "HttpResponse::bytesToWrite() this -" << this
    //         << " connection - " << m_connection << endl;
    return m_connection ? m_connection->bytesToWrite() : 0;
}

void HttpResponse::setReadBufferSize(qint64 size)
{
    m_connection->setReadBufferSize(size);
}

qint64 HttpResponse::readBufferSize()
{
    return m_connection->readBufferSize();
}

void HttpResponse::setHeader(const QString &field, const QString &value)
{
    m_headers[field] = value;
}

void HttpResponse::setSocketSendBufferSize(int size)
{
    m_connection->setSocketSendBufferSize(size);
}

int HttpResponse::getSocketSendBufferSize()
{
    return m_connection->getSocketSendBufferSize();
}

quint16 HttpResponse::localPort()
{
    return m_connection->localPort();
}

quint16 HttpResponse::peerPort()
{
    return m_connection->peerPort();
}

QString HttpResponse::peerAddress()
{
    return m_connection->peerAddress().toString();
}

void HttpResponse::writeHeaders()
{
#define WRITE_HEADER(field, value)\
    do {\
    m_connection->write(field);\
    m_connection->write(": ");\
    m_connection->write(value);\
    m_connection->write("\r\n");\
    } while(0)

    foreach(QString name, m_headers.keys())
    {
        QString value = m_headers[name];
        WRITE_HEADER(name.toLatin1(), value.toLatin1());
    }

    WRITE_HEADER("Connection", "close");
}

void HttpResponse::writeHead(int status)
{
    if( m_headerWritten ) return;
    m_connection->write(QString("HTTP/1.1 %1 %2\r\n").arg(status).arg(STATUS_CODES[status]).toLatin1());
    
    writeHeaders();

    m_connection->write("\r\n");
    m_headerWritten = true;
}

void HttpResponse::write(const QByteArray &data)
{
    if( !m_headerWritten )
    {
        qDebug() << "You MUST call writeHead() before writing body data";
        return;
    }
    m_connection->write(data);
}

qint64 HttpResponse::write(const char *data, qint64 len)
{
    if( !m_headerWritten )
    {
        qDebug() << "You MUST call writeHead() before writing body data";
        return -1;
    }
    int written = m_connection->write(data, len);
    return written;
}

void HttpResponse::write(const QString &data)
{
    write(data.toUtf8());
}

bool HttpResponse::flush()
{
    return m_connection ? m_connection->flush() : false;
}

void HttpResponse::end(const QString &data)
{
    if(!data.isEmpty())
    {
        write(data);
    }

    end();
}

void HttpResponse::end()
{
    //aDebug() << "HttpResponse::end() this -" << this
    //         << " connection - " << m_connection << endl;
    if(m_connection)
    {
        HttpConnection *conn = m_connection;
        m_connection = 0;
        conn->disconnect(this);
        conn->end();
        conn->deleteLater();

        emit done();
        deleteLater();
    }
}
