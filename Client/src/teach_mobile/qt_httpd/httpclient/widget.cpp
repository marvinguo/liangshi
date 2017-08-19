#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QUrl>

Widget::Widget(QWidget *parent)
    : QWidget(parent), m_httpc(0)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch(1);
    QHBoxLayout *actionLayout = new QHBoxLayout;
    layout->addLayout(actionLayout);

    QLabel *label = new QLabel("Input URL:");
    actionLayout->addWidget(label);
    m_urlEdit = new QLineEdit("http://xjz.wuhunews.cn/files/8782faff-3baf-4c83-b3d5-ab7c19095202.jpg");
    actionLayout->addWidget(m_urlEdit, 1);
    m_startButton = new QPushButton("Download");
    connect(m_startButton, SIGNAL(clicked()),
            this, SLOT(onStartDownload()));
    actionLayout->addWidget(m_startButton);
    m_progress = new QProgressBar;
    m_progress->setRange(0, 100);
    layout->addWidget(m_progress);
    layout->addStretch(1);
}

Widget::~Widget()
{

}

void Widget::onStartDownload()
{
    if(m_httpc) return;
    QString url = m_urlEdit->text();
    m_httpc = new CHttpClient(this);
    connect(m_httpc, SIGNAL(headerReceived(int)),
            this, SLOT(onHeaderReceived(int)));
    connect(m_httpc, SIGNAL(readyRead()),
            this, SLOT(onReadyRead()));
    connect(m_httpc, SIGNAL(finished()),
            this, SLOT(onFinished()));
    connect(m_httpc, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(onDownloadProgress(qint64,qint64)));
    connect(m_httpc, SIGNAL(error(CHttpClient::Error)),
            this, SLOT(onError(CHttpClient::Error)));
    m_httpc->download(url);
}

void Widget::onReadyRead()
{
    QByteArray data = m_httpc->readAll();
    qDebug() << "read " << data.length();
}

void Widget::onFinished()
{
    QByteArray data = m_httpc->readAll();
    qDebug() << "on finished read " << data.length();
    m_httpc->disconnect(this);
    m_httpc->deleteLater();
    m_httpc = 0;
}

void Widget::onDownloadProgress(qint64 received, qint64 total)
{
    if(total > 0)
    {
        int value = (received * 100)/total;
        m_progress->setValue(value);
    }
}

void Widget::onError(CHttpClient::Error errorCode)
{
    qDebug() << "error - " << errorCode;
    m_httpc->disconnect(this);
    m_httpc->deleteLater();
    m_httpc = 0;
}

void Widget::onHeaderReceived(int statusCode)
{
    qDebug() << "receive header, status code - " << statusCode;
}

