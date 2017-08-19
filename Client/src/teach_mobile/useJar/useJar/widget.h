#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    bool event(QEvent *e);

public slots:
    void onGet();

private:
    QLineEdit * m_urlEdit;
    QTextEdit * m_resultView;
    QLabel * m_stateLabel;
};

#endif // WIDGET_H
