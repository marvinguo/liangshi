#include "widget.h"
#include <QPainter>
#include <QThreadPool>
#include <QRunnable>
#include <QColor>
#include <QCoreApplication>
#include <QDateTime>

class ImageRunnable : public QRunnable
{
public:
    ImageRunnable(QObject *receiver)
        : m_receiver(receiver)
    {}
    ~ImageRunnable(){}

    void run()
    {
        int width = qrand() % 400 + 100;
        QImage image(width, width, QImage::Format_ARGB32);
        int red = 0;
        int increase = 0;
        for(int i = 0; i < width; i++)
        {
            red = (i / 8)*increase;
            increase += 5;
            for(int j = 0; j < width; j++)
            {
                image.setPixel(i, j, qRgb(red, 0, 0));
            }
        }

        ImageEvent *e = new ImageEvent;
        e->m_image = image;
        QCoreApplication::postEvent(m_receiver, e);
    }

    QObject *m_receiver;
};

QEvent::Type ImageEvent::m_evType = (QEvent::Type)registerEventType();

ImageEvent::ImageEvent()
    :QEvent(m_evType)
{}
ImageEvent::~ImageEvent()
{}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));\
    timer->start(1000);
    qsrand(QDateTime::currentDateTime().toTime_t());
}

Widget::~Widget()
{

}

bool Widget::event(QEvent *e)
{
    if(e->type() == ImageEvent::m_evType)
    {
        e->accept();
        m_image = ((ImageEvent*)e)->m_image;
        update();
        return true;
    }
    return QWidget::event(e);
}

void Widget::onTimeout()
{
    QThreadPool::globalInstance()->start(new ImageRunnable(this));
}

void Widget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.drawImage(0, 0, m_image);
}
