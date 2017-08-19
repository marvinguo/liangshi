#include "widget.h"
#include <QPushButton>
#include <QtAndroid>
#include <QDebug>
#include <QAndroidJniEnvironment>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QPushButton *btn = new QPushButton("ShowVersion", this);
    connect(btn, SIGNAL(clicked()),
            this, SLOT(onShowVersion()));
}

Widget::~Widget()
{

}


void Widget::onShowVersion()
{
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QString strAndroidVersion = QString::number(QtAndroid::androidSdkVersion());
    QAndroidJniObject sdk = QAndroidJniObject::fromString(strAndroidVersion);
    activity.callMethod<void>(
                "makeToast",
                "(Ljava/lang/String;)V",
                sdk.object<jstring>());
    /*
    QAndroidJniObject::callStaticMethod<void>(
                "an/qt/checkVersion/ExtendsQtWithJava",
                "makeToast",
                "(Ljava/lang/String;)V",
                sdk.object<jstring>());
    */
    QAndroidJniEnvironment env;
    if(env->ExceptionCheck()) env->ExceptionClear();
}
