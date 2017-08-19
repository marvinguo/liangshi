#include "httpclient.h"
#include <QUrl>

#define DEFAULT_USER_AGENT "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.71 Safari/537.36"


CHttpClient::CHttpClient(QObject *parent)
    : QObject(parent), m_socket(0)
    , m_statusCode(0), m_contentLength(0)
    , m_received(0), m_state(hsUnknown)
{}

CHttpClient::~CHttpClient()
{
    close();
}

void CHttpClient::download(const QString &url)
{
    if(m_socket)
    {
        emit error(AlreadyInUseError);
        return;
    }

    m_qurl = url;
    QString host = m_qurl.host();
    quint16 port = m_qurl.port(80);
    m_socket = new QTcpSocket(this);
    connect(m_socket, SIGNAL(connected()),
            this, SLOT(onSocketConnected()));
    connect(m_socket, SIGNAL(readyRead()),
            this, SLOT(onReadyRead()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(m_socket, SIGNAL(bytesWritten(qint64)),
            this, SLOT(onBytesWritten(qint64)));
    m_socket->connectToHost(host, port);
    m_state = hsConnecting;
}

QByteArray CHttpClient::readAll()
{
    QByteArray data = m_data;
    m_data.clear();
    return data;
}

void CHttpClient::onSocketConnected()
{
    QByteArray req = createRequest();
    m_socket->write(req);
    m_state = hsRequestSending;
}

void CHttpClient::onReadyRead()
{
    if(m_state == hsResponeHeaderWaiting)
    {
        processResponseHeader();
    }
    else if(m_state == hsBodyReceiving)
    {
        QByteArray data = m_socket->readAll();
        m_received += data.length();
        m_data.append(data);
        emit readyRead();
        emit downloadProgress(m_received, m_contentLength);
        if(m_received == m_contentLength)
        {
            emit finished();
            close();
        }
    }
}

void CHttpClient::onBytesWritten(qint64 len)
{
    m_state = hsResponeHeaderWaiting;
    qDebug() << "written - " << len;
}

void CHttpClient::onSocketError(QAbstractSocket::SocketError code)
{
    emit error((Error)code);
}

QByteArray CHttpClient::createRequest()
{
    QString path = m_qurl.path();
    if(path.isEmpty())
    {
        path = "/";
    }
    if(m_qurl.hasQuery())
    {
        path += m_qurl.query();
    }
    QString req = QString("GET %1 HTTP/1.1\r\n"
                          "User-Agent: %2\r\n"
                          "Accept: */*\r\n"
                          "Host: %3\r\n"
                          "Pragma:no-cache\r\n"
                          "Cache-Control:no-cache\r\n\r\n")
            .arg(path)
            .arg(DEFAULT_USER_AGENT)
            .arg(m_qurl.host());
    return req.toLatin1();
}

void CHttpClient::processResponseHeader()
{
    bool end = false;
    QString line;
    while (!end && m_socket->canReadLine())
    {
        line = QString::fromLatin1(m_socket->readLine());
        if (line == QLatin1String("\r\n") ||
                line == QLatin1String("\n")
                || line.isEmpty())
        {
            end = true;
        }
        else
        {
            m_respHeader += line;
        }
    }
    if(end)
    {
        //TODO: parse header
        QStringList headers = m_respHeader.split("\r\n", QString::SkipEmptyParts);
        int size = headers.size();
        if(size == 0)
        {
            emit error(NetworkError);
            close();
            return;
        }
        //status line { http-version status_code reason_desc }
        QStringList stats = headers.at(0).split(' ', QString::SkipEmptyParts);
        if(stats.size() < 2)
        {
            emit error(NetworkError);
            close();
            return;
        }
        m_statusCode = stats.at(1).toInt();
        for(int i = 1; i < size; i++)
        {
            if(headers.at(i).startsWith("Content-Length", Qt::CaseInsensitive))
            {
                const QString & line = headers.at(i);
                int sep = line.indexOf(':');
                if(sep != -1)
                {
                    QString strLength = line.mid(sep+1);
                    m_contentLength = strLength.trimmed().toLongLong();
                    qDebug() << "got content length - " << m_contentLength;
                }
                break;
            }
        }
        emit headerReceived(m_statusCode);
        if(m_statusCode == 200 || m_statusCode == 206)
        {
            m_state = hsBodyReceiving;
            if(m_socket->bytesAvailable()) onReadyRead();
        }
        else
        {
            m_state = hsCompleted;
            emit finished();
            close();
        }
    }
}

void CHttpClient::close()
{
    if(m_socket)
    {
        m_socket->disconnect(this);
        m_socket->deleteLater();
        m_socket = 0;
        m_state = hsUnknown;
        m_received = 0;
        m_contentLength = 0;
        m_data.clear();
        m_respHeader.clear();
        m_qurl.clear();
    }
}
