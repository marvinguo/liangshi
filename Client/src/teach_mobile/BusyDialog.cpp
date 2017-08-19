#include "BusyDialog.h"
#include "../common/YunHttp.h"
#include "../common/QHttpFileDownloader.h"
#include "LocalSave.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BusyDialog::BusyDialog(QWidget* parent):
QDialog(parent)
{
	setWindowIcon(QIcon(":/App/teach"));
	setWindowTitle(tr("正在努力升级中..."));
	Qt::WindowFlags flags = windowFlags();
	flags = flags & (~Qt::WindowContextHelpButtonHint);
	flags = flags & (~Qt::WindowCloseButtonHint);
	setWindowFlags(flags);
	setModal(true);

	QPalette pal = palette();
	pal.setColor(QPalette::Window, Qt::transparent);
	//setPalette(pal);

	//setAttribute(Qt::WA_TranslucentBackground);

	_busy = new BusyIndicator();

	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addWidget(_busy);
	setLayout(mainLayout);

	setFixedHeight(50);
	setFixedWidth(200);
}

void BusyDialog::ShowModal()
{
	QSize screenSize = QApplication::desktop()->geometry().size();
	int primaryScreenWidth = screenSize.width();
	int primaryScreeHeight = screenSize.height();
	int widgetWidth = width();
	int widgetHeight = height();
	move(primaryScreenWidth / 2 - widgetWidth / 2, primaryScreeHeight / 2 - widgetHeight / 2);
	show();
	exec();
}

void BusyDialog::Done()
{
	this->close();
}
