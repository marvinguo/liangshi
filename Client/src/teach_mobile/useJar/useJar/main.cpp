#include "widget.h"
#include <QApplication>
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <jni.h>
#include "../simpleCustomEvent.h"
#include <QDebug>

QObject *g_listener = 0;

// result: -1 failed; 1 success; 0 busy;
static void onDownloaded(JNIEnv *env, jobject thiz,int result, jstring uri, jstring data)
{
    QString qstrData;
    const char *nativeString = env->GetStringUTFChars(data, 0);
    qstrData = nativeString;
    env->ReleaseStringUTFChars(data, nativeString);
    QCoreApplication::postEvent(g_listener, new SimpleCustomEvent(result, qstrData));
}

bool registerNativeMethods()
{
    JNINativeMethod methods[] {
        {"OnDownloaded", "(ILjava/lang/String;Ljava/lang/String;)V", (void*)onDownloaded}
    };

    const char *classname = "an/qt/useJar/ExtendsQtNative";
    jclass clazz;
    QAndroidJniEnvironment env;

    QAndroidJniObject javaClass(classname);
    clazz = env->GetObjectClass(javaClass.object<jobject>());
    qDebug() << "find ExtendsQtNative - " << clazz;
    bool result = false;
    if(clazz)
    {
        jint ret = env->RegisterNatives(clazz,
                                        methods,
                                        sizeof(methods) / sizeof(methods[0]));
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

    SimpleCustomEvent::eventType();
    registerNativeMethods();

    Widget w;
    g_listener = qobject_cast<QObject*>(&w);
    w.show();

    return a.exec();
}
