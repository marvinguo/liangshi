#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <jni.h>
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include "CustomEvent.h"

QObject *main_window = 0;

static void reportGpsInfo(JNIEnv *env, jobject thiz,int result, jstring data)
{
    QString qstrData;
    const char *nativeString = env->GetStringUTFChars(data, 0);
    qstrData = nativeString;
    env->ReleaseStringUTFChars(data, nativeString);
    QCoreApplication::postEvent(main_window, new CustomEvent(result, qstrData));
    qDebug() << qstrData;
}

bool registerNativeMethods()
{
    JNINativeMethod methods[] {
        {"reportGpsInfo", "(ILjava/lang/String;)V", (void*)reportGpsInfo}
    };

    const char *classname = "com/mtn/mes/ExtendsQtNative";
    jclass clazz;
    QAndroidJniEnvironment env;
    QAndroidJniObject javaClass(classname);
    clazz = env->GetObjectClass(javaClass.object<jobject>());
    qDebug() << "find ExtendsQtNative - " << clazz;

    bool result = false;
    if(clazz) {
        jint ret = env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0]));
        env->DeleteLocalRef(clazz);
        qDebug() << "RegisterNatives return - " << ret;
        result = ret >= 0;
    }
    if(env->ExceptionCheck()) env->ExceptionClear();
    return result;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    CustomEvent::eventType();
    registerNativeMethods();
    w.show();
    main_window = qobject_cast<QObject*>(&w);
    return a.exec();
}
