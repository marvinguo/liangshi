#include "widget.h"
#include <QApplication>
#include <QFont>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QDebug>
#include "qDebug2Logcat.h"

static jstring getQtVersion(JNIEnv *env, jobject thiz)
{
    static QAndroidJniObject javaString = QAndroidJniObject::fromString(qVersion());
    return javaString.object<jstring>();
}

int main(int argc, char *argv[])
{
    installLogcatMessageHandler("checkVersion");
    QApplication a(argc, argv);
    QFont f = a.font();
    f.setPointSize(24);

    JNINativeMethod methods[] {
        {"GetQtVersion","()Ljava/lang/String;", (void*)getQtVersion}
    };

    const char *classname = "an/qt/checkVersion/ExtendsQtNative";
    jclass clazz;
    QAndroidJniEnvironment env;

    QAndroidJniObject javaClass(classname);
    clazz = env->GetObjectClass(javaClass.object<jobject>());
    qDebug() << "find ExtendsQtNative - " << clazz;
    if(clazz)
    {
        jint ret = env->RegisterNatives(clazz,
                                        methods,
                                        sizeof(methods) / sizeof(methods[0]));
        //env->DeleteLocalRef(clazz);
        qDebug() << "RegisterNatives return - " << ret;
    }
    if(env->ExceptionCheck()) env->ExceptionClear();

    Widget w;
    w.show();

    return a.exec();
}
