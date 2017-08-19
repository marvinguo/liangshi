#include "widget.h"
#include <QPushButton>
#include <QAndroidJniObject>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QPushButton *callBtn = new QPushButton("CallSettings", this);
    connect(callBtn, SIGNAL(clicked()),
            this, SLOT(onCallSettings()));
}

Widget::~Widget()
{

}

void Widget::onCallSettings()
{
    QAndroidJniObject action = QAndroidJniObject::fromString("android.settings.SETTINGS");
    QAndroidJniObject::callStaticMethod<void>(
                "an/qt/callSettings/ExtendsQtWithJava",
                "launchActivity",
                "(Ljava/lang/String;)V",
                action.object<jstring>()
                );
}
