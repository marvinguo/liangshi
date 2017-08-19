#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include "httpclient.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected slots:
    void onStartDownload();
    void onHeaderReceived(int statusCode);
    void onReadyRead();
    void onFinished();
    void onDownloadProgress(qint64 received, qint64 total);
    void onError(CHttpClient::Error code);

protected:
    QLineEdit *m_urlEdit;
    QPushButton *m_startButton;
    QProgressBar *m_progress;
    CHttpClient *m_httpc;
};

#endif // WIDGET_H
