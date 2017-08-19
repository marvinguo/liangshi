#include "ChatMainWindow.h"
#include "FriendList.h"
#include "../common/LokiClient.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ChatMainWindow::ChatMainWindow(QWidget *parent):
QWidget(parent)
{
	_ui.setupUi(this);
	Qt::WindowFlags flag = Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint;
	setWindowFlags(flag);
	setFixedSize(240, 500);

	setWindowTitle(tr("沟通"));
	setWindowIcon(QIcon(":/Resources/AppIcons/teach.png"));

	QPalette palette;
	QPixmap pixmap(":/Resources/Images/teachBackground.jpg");
	palette.setBrush(QPalette::Window, QBrush(pixmap));
	setPalette(palette);

	_ui._avatar->setStyleSheet("border-image: url(Avatars/128.bmp)}");

	/*
	_tray = new QSystemTrayIcon(this);
	_trayMenu = new QMenu(this);
	_quitAction = new QAction(tr("123"), this);
	connect(_quitAction, SIGNAL(triggered()), this, SLOT(close()));
	_trayMenu->addAction(_quitAction);
	_tray->setContextMenu(_trayMenu);
	_tray->setIcon(QIcon(":/Resources/AppIcons/teach.png"));
	_tray->show();
	*/
}

ChatMainWindow::~ChatMainWindow()
{
}

