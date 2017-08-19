#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
class QPinchGesture;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    bool event(QEvent *e);

protected:
    void paintEvent(QPaintEvent *e);
    void pinchTriggered(QPinchGesture *gesture);

protected:
    QImage m_image;
    float m_rotationAngle;
    float m_scaleFactor;
    float m_currentStepScaleFactor;
};

#endif // WIDGET_H
