#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVector>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

public:
    bool eventFilter(QObject *watched, QEvent *e);

protected slots:
    void onRegisterButton();

protected:
    void resizeEvent(QResizeEvent *e);

protected:
    QVector<QWidget*> m_subwidgets;
};

#endif // WIDGET_H
