#include "widget.h"
#include <QPainter>
#include <QtConcurrent>
#include <QFuture>
#include <QTimer>

QImage loadImage(const QString &url)
{
    QImage image(url);
    return image;
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    connect(&m_watcher, SIGNAL(finished()),
            this, SLOT(onFinished()));
    QTimer::singleShot(1000, this, SLOT(onLoad()));
}

Widget::~Widget()
{

}

void Widget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.drawImage(rect(), m_image);
}

void Widget::onLoad()
{
    QString url(":/sea.jpg");
    QFuture<QImage> future = QtConcurrent::run(loadImage, url);
    m_watcher.setFuture(future);
}

void Widget::onFinished()
{
    m_image = m_watcher.future().result();
    if(!m_image.isNull())
    {
        update();
    }
}
