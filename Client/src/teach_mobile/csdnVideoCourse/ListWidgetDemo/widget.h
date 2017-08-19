#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QComboBox>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected slots:
    void onListModeChanged(const QString &mode);

protected:
    QListWidget *m_list;
    QComboBox *m_mode;
};

#endif // WIDGET_H
