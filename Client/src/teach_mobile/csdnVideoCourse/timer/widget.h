#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected slots:
    void onTimeout();

protected:
    void timerEvent(QTimerEvent *e);

protected:
    QLabel *m_timeLabel;
    int m_timerId;
};

#endif // WIDGET_H
