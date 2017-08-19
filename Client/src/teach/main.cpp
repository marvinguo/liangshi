#include "MainWindow.h"
#include "ChatMainWindow.h"
#include "../common/LokiLoginDialog.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

#ifdef Q_OS_IOS

 #include <QtPlugin> 
Q_IMPORT_PLUGIN(QIOSIntegrationPlugin)

int qt_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	/*
	Initializes the resources specified by the .qrc file with the specified base name.
	Normally, Qt resources are loaded automatically at startup.
	The Q_INIT_RESOURCE() macro is necessary on some platforms for resources stored in a static library
	*/
	//因为在iOS上，是静态库的实现？
	Q_INIT_RESOURCE(teach);
	QApplication app(argc, argv);

	QTranslator qtTranslator;
	QString translation = "qt_zh_CN";// + QLocale::system().name();
	QString translationPath = qApp->applicationDirPath() + "/translations";//QLibraryInfo::location(QLibraryInfo::TranslationsPath);
	qtTranslator.load(translation, translationPath);
	app.installTranslator(&qtTranslator);

	LokiLoginDialog login;
	login.Login();

	LokiLoginDialog::Result result = login.GetResult();

	if(result != LokiLoginDialog::LoginSuccess)
	{
		return 0;
	}

	ChatMainWindow window;
	window.show();

	//这是以前的远程控制
	//MainWindow window;
	//window.showMaximized();
	return app.exec();
}
