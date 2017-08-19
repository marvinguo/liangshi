#include "httpConnection.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

#include "httpRequest.h"
#include "httpResponse.h"

#ifdef Q_OS_WIN
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif
#include "asyncLog.h"

HttpConnection::HttpConnection(QObject *parent)
    : QTcpSocket(parent)
    , m_request(0)
    , m_response(0)
{
    //aDebug() << "HttpConnection__this " << this << endl;
    connect(this, SIGNAL(readyRead()), this, SLOT(parseRequest()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onError(QAbstractSocket::SocketError)));
}

HttpConnection::~HttpConnection()
{
    aDebug() << "~HttpConnection__this - " << this << endl;
}

void HttpConnection::parseRequest()
{
    bool end = false;
    QString line;
    while (!end && canReadLine())
    {
        line = QString::fromLatin1(readLine());
        if (line == QLatin1String("\r\n") ||
                line == QLatin1String("\n")
                || line.isEmpty())
        {
            end = true;
        }
        else
        {
            m_reqHeader += line;
        }
    }
    if(end)
    {
        QStringList headers = m_reqHeader.split("\r\n", QString::SkipEmptyParts);
        int size = headers.size();
        if(size == 0)
        {
            emit error(NetworkError);
            close();
            return;
        }
        //method line:
        //  [GET path HTTP/1.1\r\n]
        const QString &methodLine = headers.at(0);
        QStringList params = methodLine.split(' ', QString::SkipEmptyParts);
        if(params.size() < 3)
        {
            emit error(NetworkError);
            close();
            return;
        }

        m_request = new HttpRequest(this);
        m_request->setMethod(params.at(0));
        m_request->m_url.setUrl(params.at(1));

        const QString &path = params.at(1);
        int sep = path.indexOf('?');
        if(sep != -1)
        {
            m_request->setQueryString(path.mid(sep + 1));
        }

        m_request->setVersion(params.at(2));
        m_request->m_remoteAddress = peerAddress().toString();
        m_request->m_remotePort = peerPort();

        //parse HTTP headers
        for(int i = 1; i < size; ++i)
        {
            const QString &head = headers.at(i);
            aDebug() << head << endl;
            sep = head.indexOf(':');
            if(sep != -1)
            {
                m_request->setHeader(head.left(sep).trimmed().toLower(),
                                     head.mid(sep+1).trimmed());
            }
        }

        m_response = new HttpResponse(this);
        emit newRequest(m_request, m_response);
    }
}



void HttpConnection::end()
{
    //aDebug() << "HttpConnection " << this << " end() called response"
    //         << m_response << " socket " << m_socket << endl;

    if(m_response)
    {
        m_response = 0;
    }

    disconnect(this);
    flush();
    disconnectFromHost();
    close();
    deleteLater();
}

void HttpConnection::setSocketSendBufferSize(int size)
{
#ifdef Q_OS_WIN
    setsockopt(socketDescriptor(), SOL_SOCKET, SO_SNDBUF,(const char*)&size, sizeof(int));
#else
    setsockopt(socketDescriptor(), SOL_SOCKET, SO_SNDBUF,(const void*)&size, sizeof(int));
#endif
}

int HttpConnection::getSocketSendBufferSize()
{
    int size = 0;
    int len = sizeof(int);
#ifdef Q_OS_WIN
    getsockopt(socketDescriptor(), SOL_SOCKET, SO_SNDBUF, (char*)&size, (int*)&len);
#else
    getsockopt(socketDescriptor(), SOL_SOCKET, SO_SNDBUF, (void*)&size, (socklen_t*)&len);
#endif
    return size;
}

void HttpConnection::onError(QAbstractSocket::SocketError errorCode)
{
    //aDebug() << "HttpConnection " << this << " error() called response"
    //         << m_response << " socket " << m_socket << endl;
    if(!m_response)
    {
        disconnect(this);
        close();
        deleteLater();
    }
}
