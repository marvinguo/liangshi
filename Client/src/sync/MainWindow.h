#pragma once

#include "../common/Qt.h"
#include "../common/stl.h"
#include "../common/SocketServer.h"
#include "../common/TyrProtocol.h"
#include "SyncWidget.h"
#include "MainWindowMenu.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

//QMainWindow virtual
private:
	virtual void paintEvent(QPaintEvent *e);
	virtual void contextMenuEvent(QContextMenuEvent *event);
	virtual void closeEvent(QCloseEvent *e);

private:
	void InitMenu();
	void InitMainWindow();

private slots:
	void FirstShowMenuOnTimer();
	void SyncDeviceTriggered();
	void OnNewSocketClientTriggered(SocketClient* client);
	void OnSocketClientDisconnectTriggered(SocketClient* obj);

private:
	SyncWidget* _widget;
	Menu* _menu;

	SocketServer* _server;
};

