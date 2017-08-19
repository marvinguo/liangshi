#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "sizeUtil.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    //case 1. background
    QLabel *label = new QLabel("Hello Scalable Label");
    layout->addWidget(label, 1);
    /* top right bottom left */
    label->setStyleSheet(
                "QLabel{border-image:url(:/bkgnd.9.png) 38 6 6 16;"
                " border-left-width: 16; border-top-width: 38;"
                " border-right-width: 6; border-bottom-width: 6}");

    //case 2. image
    QLabel *head = new QLabel;
    QPixmap orig(":/head.png");
    orig.setDevicePixelRatio(SizeUtil::instance().dpiFactor());
    head->setPixmap(orig);
    layout->addWidget(head);

    //case 3. text button
    QHBoxLayout *hlayout = new QHBoxLayout;
    layout->addLayout(hlayout);
    QPushButton *button = new QPushButton("Text Button");
    hlayout->addWidget(button);
    hlayout->addStretch(1);
}

Widget::~Widget()
{

}
