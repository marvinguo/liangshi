#include "httpserver.h"
//#include "httpRequest.h"
//#include "httpResponse.h"
#include "responderManager.h"
#include <QSettings>
#include <QFile>
#include <QStringList>
#include <malloc.h>
#include "utils/log.h"
#include "worker.h"
#include <QHash>
#include "utils/workerThreadTemplate.h"
#include "utils/asyncLog.h"
#include <QTcpSocket>
#ifdef WIN32
#include <winsock2.h>
#endif
#include <QTime>

QHash<int, QString> STATUS_CODES;

#define CONNECTION_COUNTERS  20
class ConnectionStatistic
{
public:
    ConnectionStatistic() : m_cursor(0)
    {
        m_time.start();
        memset(m_connCounters, 0, sizeof(int)*CONNECTION_COUNTERS);
    }

    void oneConnectionAccepted()
    {
        if(m_time.elapsed() >= 1000)
        {
            m_time.restart();
            ++m_cursor;
            if(m_cursor == CONNECTION_COUNTERS)
            {
                char buf[256]={0};
                int ret = 0;
                for(int i = 0; i < CONNECTION_COUNTERS; ++i)
                {
                    ret += sprintf(buf + ret, " %d", m_connCounters[i]);
                    m_connCounters[i] = 0;
                }
                aInfo() << "connections: " << buf << endl;
                m_cursor = 0;
            }
        }
        ++m_connCounters[m_cursor];
    }

    QTime m_time;
    int m_connCounters[CONNECTION_COUNTERS];//recent seconds
    int m_cursor;
};


class HttpServerPrivate
{
public:
    HttpServerPrivate(HttpServer * server)
        : m_server(server)
        , m_config(HttpServerConfig::instance())
        , m_workerForNextTask(1)
    {
        WorkerThread * worker;
        for(int i = 1; i <= m_config.m_threads; i++)
        {
            worker = new WorkerThread(i);
            m_workers.insert(i, worker);
            worker->start();
        }
    }

    void processConnection(qintptr fd)
    {
        WorkerThread * worker;
        int retryCount = 0;
    retry:
        for(; m_workerForNextTask <= m_config.m_threads; ++m_workerForNextTask)
        {
            worker = m_workers.value(m_workerForNextTask);
            if(worker->taskCount() < m_config.m_maxConnections)
            {
                worker->postSocket(fd);
                break;
            }
        }
        if(m_workerForNextTask > m_config.m_threads)
        {
            m_workerForNextTask = 1;
            if(++retryCount > 3)
            {
                aWarn() << "HttpServer, load too high, reject a connection" << endl;
#ifdef WIN32
                closesocket(fd);
#else
                close(fd);
#endif
                return;
            }
            QThread::msleep(100);
            goto retry;
        }
        if( ++m_workerForNextTask > m_config.m_threads )
            m_workerForNextTask = 1;
        m_statistics.oneConnectionAccepted();
    }

    HttpServer * m_server;
    HttpServerConfig &m_config;
    QHash<int, WorkerThread*> m_workers;
    int m_workerForNextTask;
    ConnectionStatistic m_statistics;
};

HttpServer::HttpServer(QObject *parent)
    : QTcpServer(parent)
    , m_d(new HttpServerPrivate(this))
{
#define STATUS_CODE(num, reason) STATUS_CODES.insert(num, reason);
// {{{
  STATUS_CODE(100, "Continue")
  STATUS_CODE(101, "Switching Protocols")
  STATUS_CODE(102, "Processing")                 // RFC 2518) obsoleted by RFC 4918
  STATUS_CODE(200, "OK")
  STATUS_CODE(201, "Created")
  STATUS_CODE(202, "Accepted")
  STATUS_CODE(203, "Non-Authoritative Information")
  STATUS_CODE(204, "No Content")
  STATUS_CODE(205, "Reset Content")
  STATUS_CODE(206, "Partial Content")
  STATUS_CODE(207, "Multi-Status")               // RFC 4918
  STATUS_CODE(300, "Multiple Choices")
  STATUS_CODE(301, "Moved Permanently")
  STATUS_CODE(302, "Moved Temporarily")
  STATUS_CODE(303, "See Other")
  STATUS_CODE(304, "Not Modified")
  STATUS_CODE(305, "Use Proxy")
  STATUS_CODE(307, "Temporary Redirect")
  STATUS_CODE(400, "Bad Request")
  STATUS_CODE(401, "Unauthorized")
  STATUS_CODE(402, "Payment Required")
  STATUS_CODE(403, "Forbidden")
  STATUS_CODE(404, "Not Found")
  STATUS_CODE(405, "Method Not Allowed")
  STATUS_CODE(406, "Not Acceptable")
  STATUS_CODE(407, "Proxy Authentication Required")
  STATUS_CODE(408, "Request Time-out")
  STATUS_CODE(409, "Conflict")
  STATUS_CODE(410, "Gone")
  STATUS_CODE(411, "Length Required")
  STATUS_CODE(412, "Precondition Failed")
  STATUS_CODE(413, "Request Entity Too Large")
  STATUS_CODE(414, "Request-URI Too Large")
  STATUS_CODE(415, "Unsupported Media Type")
  STATUS_CODE(416, "Requested Range Not Satisfiable")
  STATUS_CODE(417, "Expectation Failed")
  STATUS_CODE(418, "I\"m a teapot")              // RFC 2324
  STATUS_CODE(422, "Unprocessable Entity")       // RFC 4918
  STATUS_CODE(423, "Locked")                     // RFC 4918
  STATUS_CODE(424, "Failed Dependency")          // RFC 4918
  STATUS_CODE(425, "Unordered Collection")       // RFC 4918
  STATUS_CODE(426, "Upgrade Required")           // RFC 2817
  STATUS_CODE(500, "Internal Server Error")
  STATUS_CODE(501, "Not Implemented")
  STATUS_CODE(502, "Bad Gateway")
  STATUS_CODE(503, "Service Unavailable")
  STATUS_CODE(504, "Gateway Time-out")
  STATUS_CODE(505, "HTTP Version not supported")
  STATUS_CODE(506, "Variant Also Negotiates")    // RFC 2295
  STATUS_CODE(507, "Insufficient Storage")       // RFC 4918
  STATUS_CODE(509, "Bandwidth Limit Exceeded")
  STATUS_CODE(510, "Not Extended")                // RFC 2774
// }}}
  (void*)ResponderCreatorFactory::instance();
}

HttpServer::~HttpServer()
{
}

void HttpServer::work(void *data)
{
    if( listen(m_d->m_config.m_address, m_d->m_config.m_port) )
    {
        aInfo() << "httpserver started on port " << serverPort() << endl;
    }
    else
    {
        aWarn() << "could not start httpserver: " << errorString() << endl;
    }
}

void HttpServer::incomingConnection(qintptr fd)
{
    //aDebug() << "incomingConnection " << fd << endl;
    m_d->processConnection(fd);
}


void startServer()
{
    WorkerHostThread<HttpServer> *serverThread =
            new WorkerHostThread<HttpServer>(0);
    serverThread->start();
}

