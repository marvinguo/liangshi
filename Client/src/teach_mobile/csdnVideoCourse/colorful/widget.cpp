#include "widget.h"
#include <QSensorGestureManager>
#include <QDateTime>
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent), m_color(Qt::red)
    , m_shakeGesture(0)
{
    qsrand(QDateTime::currentDateTime().toTime_t());

    QSensorGestureManager mgr;
    QStringList gestureIds = mgr.gestureIds();
    if(gestureIds.contains("QtSensors.shake"))
    {
        m_shakeGesture = new QSensorGesture(QStringList("QtSensors.shake"), this);
        connect(m_shakeGesture, SIGNAL(shake()),
                this, SLOT(onShake()));
        m_shakeGesture->startDetection();
    }
}

Widget::~Widget()
{
    if(m_shakeGesture) m_shakeGesture->stopDetection();
}

void Widget::onShake()
{
    m_color = QColor::fromRgb(qrand()%256, qrand()%256, qrand()%256);
    update();
}

void Widget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.fillRect(rect(), m_color);
}
