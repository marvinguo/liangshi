#include "widget.h"
#include <QTimerEvent>
#include <QDateTime>
#include <QTimer>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_timeLabel = new QLabel(this);
    //m_timerId = startTimer(1000);
    QTimer::singleShot(1000, this, SLOT(onTimeout()));
}

Widget::~Widget()
{

}

void Widget::onTimeout()
{
    QDateTime current = QDateTime::currentDateTime();
    QString strTime = current.toString("hh:mm:ss");
    m_timeLabel->setText(strTime);
    m_timeLabel->adjustSize();
}

void Widget::timerEvent(QTimerEvent *e)
{
    if(m_timerId == e->timerId())
    {
        QDateTime current = QDateTime::currentDateTime();
        QString strTime = current.toString("hh:mm:ss");
        m_timeLabel->setText(strTime);
        m_timeLabel->adjustSize();
        e->accept();
        killTimer(m_timerId);
        m_timerId = 0;
    }
    QWidget::timerEvent(e);
}

