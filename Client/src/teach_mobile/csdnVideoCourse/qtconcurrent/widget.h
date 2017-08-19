#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
#include <QFutureWatcher>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    QSize sizeHint() const { return QSize(800, 600); }

protected:
    void paintEvent(QPaintEvent *e);

protected slots:
    void onLoad();
    void onFinished();

protected:
    QImage m_image;
    QFutureWatcher<QImage> m_watcher;
};

#endif // WIDGET_H
