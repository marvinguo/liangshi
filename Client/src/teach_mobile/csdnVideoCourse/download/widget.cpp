#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QUrl>
/*
 * http://pic.nipic.com/2008-03-11/200831115141343_2.jpg
 * http://pic54.nipic.com/file/20141205/11284670_092924760001_2.jpg
 */

Widget::Widget(QWidget *parent)
    : QWidget(parent), m_reply(0), m_nam(this)
    , m_file(0)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch(1);
    QHBoxLayout *actionLayout = new QHBoxLayout;
    layout->addLayout(actionLayout);

    QLabel *label = new QLabel("Input URL:");
    actionLayout->addWidget(label);
    m_urlEdit = new QLineEdit("http://pic53.nipic.com/file/20141125/11284670_091803594000_2.jpg");
    actionLayout->addWidget(m_urlEdit, 1);
    m_startButton = new QPushButton("Download");
    connect(m_startButton, SIGNAL(clicked()),
            this, SLOT(onStartDownload()));
    actionLayout->addWidget(m_startButton);
    m_progress = new QProgressBar;
    m_progress->setRange(0, 100);
    layout->addWidget(m_progress);
    layout->addStretch(1);
    QFile file("d:/temp/config.txt");
    if(file.open(QFile::ReadOnly))
    {
        QTextStream ts(&file);
        m_baseDir = ts.readLine();
        qDebug() << "basedir - " << m_baseDir;
        file.close();
    }
}

Widget::~Widget()
{

}

void Widget::onStartDownload()
{
    QUrl qurl(m_urlEdit->text());
    QNetworkRequest req(qurl);
    m_reply = m_nam.get(req);
    connect(m_reply, SIGNAL(readyRead()),
            this, SLOT(onReadyRead()));
    connect(m_reply, SIGNAL(finished()),
            this, SLOT(onFinished()));
    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(onDownloadProgress(qint64,qint64)));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(onError(QNetworkReply::NetworkError)));
    QString fileName = qurl.fileName();
    if(fileName.isEmpty()) fileName = "download";
    fileName = QString("%1%2").arg(m_baseDir).arg(fileName);
    m_file = new QFile(fileName);
    if(m_file->open(QFile::WriteOnly))
    {

    }
    else
    {
        qDebug() << "open file error - " << m_file->errorString();
        delete m_file;
        m_file = 0;
    }
}

void Widget::onReadyRead()
{
    QByteArray data = m_reply->readAll();
    if(m_file)
    {
        m_file->write(data);
    }
    qDebug() << "read " << data.length();
}

void Widget::onFinished()
{
    QByteArray data = m_reply->readAll();
    if(data.length())
    {
        if(m_file)m_file->write(data);
    }
    if(m_file)
    {
        m_file->close();
        delete m_file;
        m_file = 0;
    }
    qDebug() << "read " << data.length();
    m_reply->disconnect(this);
    m_reply->deleteLater();
    m_reply = 0;
}

void Widget::onDownloadProgress(qint64 received, qint64 total)
{
    if(total > 0)
    {
        int value = (received * 100)/total;
        m_progress->setValue(value);
    }
}

void Widget::onError(QNetworkReply::NetworkError errorCode)
{
    qDebug() << "error - " << errorCode;
    m_reply->disconnect(this);
    m_reply->deleteLater();
    m_reply = 0;
}
