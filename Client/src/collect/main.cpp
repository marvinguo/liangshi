#include "MetroMainWidget.h"
#include "../hera/HeraMainWidget.h"
#include "../hera/Hera.h"
#ifdef _DEBUG
#include <vld.h>
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	//动态库不不要这个了
	//Q_INIT_RESOURCE(hera);
	app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#ifdef _DEBUG
	VLDEnable();
	int * aa = new int[100];
#endif

	HeraSingletons singletons;
	QFontDatabase::addApplicationFont(qApp->applicationDirPath() + "/Browser/Font/DroidSansFallback.ttf");

	QFile file(":/qss/default");
	file.open(QFile::ReadOnly);
	QString styleSheet = QString::fromLatin1(file.readAll());
	file.close();
	qApp->setStyleSheet(styleSheet);

	QTranslator qtTranslator;
	QString translation = "qt_zh_CN";// + QLocale::system().name();
	QString translationPath = qApp->applicationDirPath() + "/translations";//QLibraryInfo::location(QLibraryInfo::TranslationsPath);
	qtTranslator.load(translation, translationPath);
	app.installTranslator(&qtTranslator);

//	QStyle * android=QStyleFactory::create("Android");
//	qApp->setStyle(android);


	QStringList arguments = QCoreApplication::arguments();
	if(arguments.size() >= 2)
	{
		HeraMainWidget* hera = HeraMainWidget::LoadScFile(arguments[1]);
		hera->ShowModal();
		app.exec();
	}
	else 
	{
		MetroMainWidget widget;
		widget.showMaximized();
		app.exec();
	}
}
