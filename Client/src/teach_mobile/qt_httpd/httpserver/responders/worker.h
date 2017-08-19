#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include <QEvent>
#include <QPointer>
#include <QList>
#include "responderManager.h"
#include <QTcpSocket>

class WorkerThread : public QThread
{
public:
    WorkerThread(int workerId, QObject *parent = 0);
    ~WorkerThread();

    void postSocket(qintptr sock);

    int taskCount(){ return m_taskCount; }

protected:
    void run();
    bool event(QEvent *event);

private:
    int m_id;
    int m_taskCount;
    QPointer<QObject> m_worker;
    QList<qintptr> m_sockets;
};

class Worker : public QObject
{
    Q_OBJECT
    friend class WorkerThread;
public:
    Worker();
    ~Worker();

    bool event(QEvent *e);

protected slots:
    void onNewRequest(HttpRequest*, HttpResponse*);
    void onSocketError(QAbstractSocket::SocketError socketError);
    void onDone();

private:
    void processQueuedConnections();
    void processConnection(qintptr fd);

private:
    ResponderManager m_responderManager;
    QList<qintptr> m_sockets;
};

#endif // WORKER_H
