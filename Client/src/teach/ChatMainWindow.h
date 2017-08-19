#pragma once

#include "../common/Qt.h"
#include "ui_ChatMainWindow.h"

class ChatMainWindow : public QWidget
{
	Q_OBJECT
public:
	ChatMainWindow(QWidget *parent = 0);
	~ChatMainWindow();

private:
	Ui::ChatMainWindow _ui;
	QMenu* _trayMenu;
	QSystemTrayIcon* _tray;
	QAction* _quitAction;
};

