#include "../common/Qt.h"
#ifdef _WIN32
#include <vld.h>
#endif
#include "qDebug2Logcat.h"
#include "MainWindow.h"
#include "LoginActivity.h"
#include "../hera/Hera.h"
#include "AndroidCamera.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	installLogcatMessageHandler("liangshi_teach");

#ifdef _WIN32
	VLDEnable();
	int * aa = new int[100];
#endif
	HeraSingletons singletons;
	AndroidCamera camera;

	qDebug() << "main()";
//	QCoreApplication::setApplicationVersion(QT_VERSION_STR);
//	QCoreApplication::setApplicationName(QStringLiteral("良师益友"));
//	QGuiApplication::setApplicationDisplayName(QStringLiteral("Image Gestures Example"));

	QFile file(":/qss/mobile");
	file.open(QFile::ReadOnly);
	QString styleSheet = QString::fromLatin1(file.readAll());
	qApp->setStyleSheet(styleSheet);

#ifdef _WIN32
	QTranslator qtTranslator;
	QString translation = "qt_zh_CN";// + QLocale::system().name();
	QString translationPath = qApp->applicationDirPath() + "/translations";//QLibraryInfo::location(QLibraryInfo::TranslationsPath);
	qtTranslator.load(translation, translationPath);
	app.installTranslator(&qtTranslator);
#endif

#ifdef Q_OS_ANDROID
	//QTranslator qtTranslator;
	//qtTranslator.load("assets:/qt_zh_CN");
	//app.installTranslator(&qtTranslator);
#endif

	/*
	QQmlApplicationEngine engine(QUrl("qrc:/main.qml"));
	QObject *topLevel = engine.rootObjects().value(0);
	QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
	window->show();

	QQuickView view;
	view.connect(view.engine(), SIGNAL(quit()), &app, SLOT(quit()));
	view.setResizeMode(QQuickView::SizeRootObjectToView);
	view.setSource(QUrl("qrc:/qml/main"));
	if (QGuiApplication::platformName() == QLatin1String("android") ||
		QGuiApplication::platformName() == QLatin1String("ios"))
	{
		view.showFullScreen();
	}
	else
	{
		view.show();
	}
		*/

	MainWindow window;

	/*
	QPixmap pixmap(":/App/background");
	QSplashScreen splash(pixmap);
	splash.show();

	QTimer::singleShot(10000, &splash, SLOT(close()));
	QTimer::singleShot(10000, &window, SLOT(show()));
	*/

	//LoginWidget window(NULL);
#ifdef Q_OS_WIN32
	window.resize(400, 700);
	window.show();
#else
	window.showMaximized();
#endif
	return app.exec();
}

/*
QElapsedTimer timer;
timer.start();
//slowOperation1();
qDebug() << "The slow operation took" << timer.nsecsElapsed() << "nanoseconds";
*/

/*!
    \property QGuiApplication::platformName
    \brief The name of the underlying platform plugin.
 
    The QPA platform plugins are located in \c {qtbase\src\plugins\platforms}.
    At the time of writing, the following platform plugin names are supported:
 
    \list
        \li \c android
        \li \c cocoa is a platform plugin for Mac OS X.
        \li \c directfb
        \li \c eglfs is a platform plugin for running Qt5 applications on top of
            EGL and  OpenGL ES 2.0 without an actual windowing system (like X11
            or Wayland). For more information, see \l{EGLFS}.
        \li \c ios
        \li \c kms is an experimental platform plugin using kernel modesetting
            and \l{http://dri.freedesktop.org/wiki/DRM}{DRM} (Direct Rendering
            Manager).
        \li \c linuxfb writes directly to the framebuffer. For more information,
            see \l{LinuxFB}.
        \li \c minimal is provided as an examples for developers who want to
            write their own platform plugins. However, you can use the plugin to
            run GUI applications in environments without a GUI, such as servers.
        \li \c minimalegl is an example plugin.
        \li \c offscreen
        \li \c openwfd
        \li \c qnx
        \li \c windows
        \li \c xcb is the X11 plugin used on regular desktop Linux platforms.
    \endlist
 
    For more information about the platform plugins for embedded Linux devices,
    see \l{Qt for Embedded Linux}.
*/