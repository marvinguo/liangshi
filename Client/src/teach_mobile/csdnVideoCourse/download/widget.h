#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QTextStream>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected slots:
    void onStartDownload();
    void onReadyRead();
    void onFinished();
    void onDownloadProgress(qint64 received, qint64 total);
    void onError(QNetworkReply::NetworkError errorCode);

protected:
    QLineEdit *m_urlEdit;
    QPushButton *m_startButton;
    QProgressBar *m_progress;
    QNetworkAccessManager m_nam;
    QNetworkReply *m_reply;
    QFile *m_file;
    QString m_baseDir;
};

#endif // WIDGET_H
