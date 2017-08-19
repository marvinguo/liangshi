#pragma once
#ifndef _QT_HEADER_
#define _QT_HEADER_

//#define WIN32_LEAN_AND_MEAN
//#define _WINSOCKAPI_

#define QT_NO_EMIT

#include <QtWidgets>
#include <QtConcurrent/QtConcurrent>

#ifdef _LiangshiNetwork
	#include <QtNetwork>
#endif

#ifdef _LiangshiScript
	#include <QtScript>
	#include <QtScriptTools>
#endif

#ifdef _LiangshiWebkit
	#include <QtWebKit>
	#include <QtWebKitWidgets>
	#include <QWebPluginFactory>
#endif

#ifdef _LiangshiQML
	#include <QQmlEngine>
	#include <QQuickView>
	//’‚ «qt quick 1.0
	//#include <QtDeclarative>
#endif

#ifdef _LiangshiQMultimedia
	#include <QCamera>
	#include <QCameraImageCapture>
	#include <QMediaRecorder>
	#include <QCameraViewfinder>
#endif

#ifdef _LiangshiActiveX
	#include <qaxobject>
#endif

#ifdef Q_OS_ANDROID
	#include <QtAndroidExtras/QtAndroid>
	#include <QtAndroidExtras/QAndroidActivityResultReceiver>
	#include <QtAndroidExtras/QAndroidJniEnvironment>
#endif

template <class T> class VPtr
{
public:
	static T* asPtr(QVariant v)
	{
		return  (T *) v.value<void *>();
	}

	static QVariant asQVariant(T* ptr)
	{
		return qVariantFromValue((void *) ptr);
	}
};
/*
MyClass *p;
QVariant v = VPtr<MyClass>::asQVariant(p);

MyClass *p1 = VPtr<MyClass>::asPtr(v);
*/

#endif /* _QT_HEADER_ */