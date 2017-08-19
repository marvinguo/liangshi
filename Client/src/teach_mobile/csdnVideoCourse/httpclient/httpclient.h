#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QTcpSocket>
#include <QUrl>

class CHttpClient : public QObject
{
    Q_OBJECT
public:
    enum Error {
        ConnectionRefusedError,
        RemoteHostClosedError,
        HostNotFoundError,
        SocketAccessError,
        SocketTimeoutError,
        NetworkError,
        AlreadyInUseError
    };

    enum HttpState {
        hsUnknown = 0,
        hsConnecting = 1,
        hsRequestSending,
        hsResponeHeaderWaiting,
        hsBodyReceiving,
        hsCompleted
    };

    CHttpClient(QObject *parent = 0);
    ~CHttpClient();

    void download(const QString &url);
    QByteArray readAll();
    int statusCode();
    qint64 contentLength();

signals:
    void connected();
    void headerReceived(int statusCode);
    void readyRead();
    void finished();
    void error(CHttpClient::Error code);
    void downloadProgress(qint64 received, qint64 total);

protected slots:
    void onSocketConnected();
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError code);
    void onBytesWritten(qint64 len);

private:
    QByteArray createRequest();
    void processResponseHeader();
    void close();

protected:
    QTcpSocket *m_socket;
    QUrl m_qurl;
    QByteArray m_data;
    QString m_respHeader;
    int m_statusCode;
    qint64 m_contentLength;
    qint64 m_received;
    HttpState m_state;
};

#endif // HTTPCLIENT_H
