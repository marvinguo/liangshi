#include "worker.h"
#include <QCoreApplication>
#include <QTcpSocket>
#include "httpConnection.h"
#include "httpRequest.h"
#include "httpResponse.h"
#include "../utils/asyncLog.h"

class TaskEvent : public QEvent
{
public:
    TaskEvent(qintptr socket)
        : QEvent(evType())
        , m_socket(socket)
    {}

    static QEvent::Type evType()
    {
        if(s_evType == QEvent::None)
        {
            s_evType = (QEvent::Type)registerEventType();
        }
        return s_evType;
    }

    qintptr m_socket;

private:
    static QEvent::Type s_evType;
};
QEvent::Type TaskEvent::s_evType = QEvent::None;

Worker::Worker() : m_responderManager(this)
{
    connect(&m_responderManager, SIGNAL(oneResponderDestroyed()), this, SLOT(onDone()));
}

Worker::~Worker()
{
}

bool Worker::event(QEvent *e)
{
    QEvent::Type t = e->type();
    if(t == TaskEvent::evType())
    {
        TaskEvent * re = (TaskEvent*)e;
        if(!re->m_socket)
        {
            processQueuedConnections();
        }
        else
        {
            processConnection(re->m_socket);
        }
        return true;
    }
    return QObject::event(e);
}

void Worker::onNewRequest(HttpRequest *req, HttpResponse *resp)
{
    m_responderManager.handleRequest(req, resp);
}

void Worker::onSocketError(QAbstractSocket::SocketError socketError)
{
    //delete HttpConnection
    QCoreApplication::postEvent(QThread::currentThread(),
                                new TaskEvent(0));
}

void Worker::onDone()
{
    QCoreApplication::postEvent(QThread::currentThread(),
                                new TaskEvent(0));
}

void Worker::processQueuedConnections()
{
    int count = m_sockets.size();
    if(count > 0)
    {
        for(int i = 0; i < count; i++)
            processConnection(m_sockets.at(i));
        m_sockets.clear();
    }
}

void Worker::processConnection(qintptr sock)
{
    if(sock < 0) return;
    //setup http connection
    HttpConnection *conn = new HttpConnection(this);
    conn->setSocketDescriptor(sock);
    connect(conn, SIGNAL(newRequest(HttpRequest*, HttpResponse*)),
            this, SLOT(onNewRequest(HttpRequest*, HttpResponse*)));
    connect(conn, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onSocketError(QAbstractSocket::SocketError)));
}

WorkerThread::WorkerThread(int id, QObject *parent)
    : QThread(parent)
    , m_id(id)
    , m_taskCount(0)
{

}

WorkerThread::~WorkerThread()
{
}

void WorkerThread::postSocket(qintptr sock)
{
    m_taskCount++;
    //aDebug() << "worker " << m_id << " postSocket task count " << m_taskCount << endl;
    if(m_worker)
    {
        QCoreApplication::postEvent(m_worker, new TaskEvent(sock));
    }
    else
    {
        m_sockets.append(sock);
    }
}

bool WorkerThread::event(QEvent *e)
{
    if(e->type() == TaskEvent::evType())
    {
        m_taskCount--;
        //aDebug() << "worker " << m_id << " taskDone task count " << m_taskCount << endl;
        return true;
    }
    QThread::event(e);
}

void WorkerThread::run()
{
    Worker *worker = new Worker();
    m_worker = worker;
    if(m_sockets.size() > 0)
    {
        worker->m_sockets = m_sockets;
        m_sockets.clear();
        QCoreApplication::postEvent(worker, new TaskEvent(0));
    }
    exec();
}
