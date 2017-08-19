#include "mainwindow.h"
#include "../hera/Hera.h"
#ifdef _DEBUG
#include <vld.h>
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

void myMessageOutput(QtMsgType type, const QMessageLogContext&, const QString& msg)
{
	switch (type) {
	case QtDebugMsg:
		fprintf(stderr, "Debug: %s\n", msg);
		break;
	case QtWarningMsg:
		fprintf(stderr, "Warning: %s\n", msg);
		break;
	case QtCriticalMsg:
		fprintf(stderr, "Critical: %s\n", msg);
		break;
	case QtFatalMsg:
		fprintf(stderr, "Fatal: %s\n", msg);
		abort();
	}

	QFile file("s:/111.txt");
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream ts(&file);
	ts<<msg<<endl;
	file.flush();
	file.close();
}

int main(int argc, char *argv[])
{
	//qInstallMessageHandler(myMessageOutput);
	QApplication app(argc, argv);
	//Q_INIT_RESOURCE(hera);

#ifdef _DEBUG
	VLDEnable();
	int * aa = new int[100];
#endif
	HeraSingletons singletons;
	QFontDatabase::addApplicationFont(qApp->applicationDirPath() + "/Browser/Font/DroidSansFallback.ttf");

	QProcess* process = new QProcess();
	process->setWorkingDirectory(QCoreApplication::applicationDirPath());
	process->start("regfile.exe");

	QTranslator qtTranslator;
	QString translation = "qt_zh_CN";// + QLocale::system().name();
	QString translationPath = qApp->applicationDirPath() + "/translations";//QLibraryInfo::location(QLibraryInfo::TranslationsPath);
	qtTranslator.load(translation, translationPath);
	app.installTranslator(&qtTranslator);

	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));

	MainWindow window;
	window.showMaximized();
	return app.exec();
}
