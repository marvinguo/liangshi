#include "widget.h"
#include <QVBoxLayout>
#include <QLabel>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    QLabel *hello = new QLabel("Hello Qt on Android");
    layout->addWidget(hello);
    setLayout(layout);
}

Widget::~Widget()
{

}
