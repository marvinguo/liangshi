#include "widget.h"
#include <QThread>
#include <QVBoxLayout>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_progress = new QProgressBar;
    layout->addWidget(m_progress, 0, Qt::AlignCenter);

    DownloadThread *t = new DownloadThread(m_progress);
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    t->start();
}

Widget::~Widget()
{

}


Worker::Worker(QObject *parent)
    : QObject(parent), m_progress(0)
{
    connect(&m_timer, SIGNAL(timeout()),
            this, SLOT(onTimeout()));
    m_timer.start(100);
}

Worker::~Worker()
{
    qDebug() << "Worker::~Workder()";
}

void Worker::onTimeout()
{
    if(m_progress < 100)
    {
        m_progress++;
        emit progressChanged(m_progress);
    }
    else
    {
        m_timer.stop();
        QThread::currentThread()->quit();
        delete this;
    }
}

DownloadThread::DownloadThread(QObject *receiver, QObject *parent)
    : QThread(parent), m_receiver(receiver)
{

}

DownloadThread::~DownloadThread()
{}

void DownloadThread::run()
{
    Worker * worker = new Worker;
    connect(worker, SIGNAL(progressChanged(int)),
            m_receiver, SLOT(setValue(int)));
    exec();
}
