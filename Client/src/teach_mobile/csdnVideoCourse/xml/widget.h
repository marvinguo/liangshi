#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTableWidget>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void readContacts();

protected:
    QTableWidget *m_contacts;
};

#endif // WIDGET_H
