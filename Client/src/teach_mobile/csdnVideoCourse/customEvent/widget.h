#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "CustomEvent.h"
#include <QLabel>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    bool event(QEvent *e);

protected slots:
    void onTimeout();

protected:
    QLabel *m_label;
};

#endif // WIDGET_H
