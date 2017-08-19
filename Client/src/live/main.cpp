#include "../hera/HeraMainWidget.h"
#include "../hera/ChineseMessageBox.h"
#include "../hera/Hera.h"
#include "../common/config.h"
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

	//检查单例，老师学生，只能一个时间打开一个？讨论一下
#ifdef QT_NO_DEBUG
	QSharedMemory singleSharedMemory;
	singleSharedMemory.setKey(LIVE_SINGLE_KEY);
	if (singleSharedMemory.attach())
	{
		ChineseMessageBox::Show(QObject::tr("错误"), QObject::tr("只能在一个时间开启一个实时课堂"), ChineseMessageBox::OK, ChineseMessageBox::Error);
		qApp->quit();
		return 0;
	}
	else
	{
		singleSharedMemory.create(1);
	}
#endif

	QTranslator qtTranslator;
	QString translation = "qt_zh_CN";// + QLocale::system().name();
	QString translationPath = qApp->applicationDirPath() + "/translations";//QLibraryInfo::location(QLibraryInfo::TranslationsPath);
	qtTranslator.load(translation, translationPath);
	app.installTranslator(&qtTranslator);

	HeraMainWidget* widget = new HeraMainWidget(NewableDocType_Live);

	int width = BOARD_WIDTH;
	int height = BOARD_HEIGHT;
	QDesktopWidget *desktop = QApplication::desktop();
	if(desktop->availableGeometry().width() < width)
		width = desktop->availableGeometry().width();
	if(desktop->availableGeometry().height() < height)
		height = desktop->availableGeometry().height();
	widget->resize(width, height);

	widget->ShowModal();

	return app.exec();
}
