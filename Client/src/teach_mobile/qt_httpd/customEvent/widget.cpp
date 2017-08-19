#include "widget.h"
#include <QEvent>
#include <QTimer>
#include <QDateTime>
#include <QCoreApplication>
#include <QThread>

class SimpleThread: public QThread
{
public:
    SimpleThread(QObject *receiver)
        : m_receiver(receiver)
    {}

    void run()
    {
        while(1)
        {
            QThread::msleep(1000);
            CustomEvent *ce = new CustomEvent(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            QCoreApplication::postEvent(m_receiver, ce);
        }
    }

    QObject *m_receiver;
};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_label = new QLabel(this);

    SimpleThread *t = new SimpleThread(this);
    t->start();
}

Widget::~Widget()
{

}

bool Widget::event(QEvent *e)
{
    if(e->type() == CustomEvent::evType())
    {
        CustomEvent *ce = (CustomEvent*)e;
        m_label->setText(ce->m_msg);
        m_label->adjustSize();
        e->accept();
        return true;
    }
    return QWidget::event(e);
}
