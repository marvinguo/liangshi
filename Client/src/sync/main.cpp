#include <QtWidgets/QApplication>
#include "MainWindow.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

//同步课件
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QTranslator qtTranslator;
	QString translation = "qt_" + QLocale::system().name();
	QString translationPath = qApp->applicationDirPath() + "/translations";//QLibraryInfo::location(QLibraryInfo::TranslationsPath);
	qtTranslator.load(translation, translationPath);
	app.installTranslator(&qtTranslator);

	MainWindow window;
	window.showMaximized();
	return app.exec();
}
