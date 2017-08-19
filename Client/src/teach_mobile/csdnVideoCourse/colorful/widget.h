#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QColor>
#include <QSensorGesture>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected slots:
    void onShake();

protected:
    void paintEvent(QPaintEvent *e);

private:
    QColor m_color;
    QSensorGesture *m_shakeGesture;
};

#endif // WIDGET_H
