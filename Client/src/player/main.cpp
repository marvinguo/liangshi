#include "../hera/HeraMainWidget.h"
#include "../hera/Hera.h"
#include "HiddenMainWidget.h"
#ifdef _DEBUG
#include <vld.h>
#endif
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	//Q_INIT_RESOURCE(hera);

#ifdef _DEBUG
	VLDEnable();
	int * aa = new int[100];
#endif

	HeraSingletons singletons;
	QFontDatabase::addApplicationFont(qApp->applicationDirPath() + "/Browser/Font/DroidSansFallback.ttf");

	QTranslator qtTranslator;
	QString translation = "qt_zh_CN";// + QLocale::system().name();
	QString translationPath = qApp->applicationDirPath() + "/translations";//QLibraryInfo::location(QLibraryInfo::TranslationsPath);
	qtTranslator.load(translation, translationPath);
	app.installTranslator(&qtTranslator);

	//bool hello = app.quitOnLastWindowClosed();
	//app.setQuitOnLastWindowClosed(false);
	
	HiddenMainWidget widget;
	widget.hide();
	return app.exec();
}
