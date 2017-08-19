#include "widget.h"
#include <QGesture>
#include <QGestureEvent>
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      m_image(":/test.jpg"),
      m_rotationAngle(0),
      m_scaleFactor(1),
      m_currentStepScaleFactor(1)
{
    grabGesture(Qt::PinchGesture);
}

Widget::~Widget()
{

}

bool Widget::event(QEvent *e)
{
    QEvent::Type t = e->type();
    switch(t)
    {
    case QEvent::Gesture:
        QGestureEvent * ge;
        QGesture *pinch;
        ge = static_cast<QGestureEvent*>(e);
        if(pinch = ge->gesture(Qt::PinchGesture))
        {
            pinchTriggered(static_cast<QPinchGesture *>(pinch));
            return true;
        }
        break;
    default:
        break;
    }

    return QWidget::event(e);
}

void Widget::paintEvent(QPaintEvent* e)
{
    QPainter p(this);

    if(!m_image.isNull())
    {
        float iw = m_image.width();
        float ih = m_image.height();
        float wh = height();
        float ww = width();

        p.translate(ww/2, wh/2);
        p.rotate(m_rotationAngle);
        p.scale(m_currentStepScaleFactor * m_scaleFactor, m_currentStepScaleFactor * m_scaleFactor);
        p.translate(-iw/2, -ih/2);
        p.drawImage(0, 0, m_image);
    }
}

void Widget::pinchTriggered(QPinchGesture *gesture)
{
    QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();

    if (changeFlags & QPinchGesture::RotationAngleChanged)
    {
        qreal value = gesture->property("rotationAngle").toReal();
        qreal lastValue = gesture->property("lastRotationAngle").toReal();
        m_rotationAngle += value - lastValue;
    }
    if (changeFlags & QPinchGesture::ScaleFactorChanged)
    {
        qreal value = gesture->scaleFactor();
        m_currentStepScaleFactor *= value;
    }
    if (gesture->state() == Qt::GestureFinished)
    {
        m_scaleFactor *= m_currentStepScaleFactor;
        m_currentStepScaleFactor = 1;
    }
    update();
}
