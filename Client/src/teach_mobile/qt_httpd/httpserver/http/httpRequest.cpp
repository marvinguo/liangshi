#include "httpRequest.h"
#include "httpConnection.h"
#include <QHostAddress>
#include "asyncLog.h"

HttpRequest::HttpRequest(HttpConnection *connection, QObject *parent)
    : QObject(parent)
    , m_connection(connection)
    , m_url("http://localhost/")
{
}

HttpRequest::~HttpRequest()
{
    aDebug() << "~HttpRequest";
}

QString HttpRequest::localAddress() const
{
    return m_connection->localAddress().toString();
}

