#pragma once

#include "../common/Qt.h"
#include "../common/stl.h"
#include "../common/Menu.h"
#include "../common/ThorProtocol.h"
#include "../common/SocketClient.h"
#include "../common/TyrProtocol.h"
#include "SyncHandler.h"
#include "TeachMessageBox.h"
#include "ControlWidget.h"

//teach里有2个protocol
//TyrProtocol在MainWindow里
//ControlProtocol在ControlWidget里

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
	virtual void mousePressEvent(QMouseEvent *e);	
    virtual void closeEvent(QCloseEvent *e);
	virtual bool event(QEvent *);    

private:
	void InitMenu();
	void InitMainWindow();

private Q_SLOTS:
	void FirstShowMenuOnTimer();
	void OnClassTriggered();
	void OnSyncCoursewareTriggered();
	void OnOnGetServerInfoTriggered(ThorProtocolType type, QString ip, int port);
	void OnClientDisconnectTriggered(SocketClient*);
	void OnCancelWaitingForConnectionTriggered();
	void OnCancelSyncingTriggered();

private:
	Menu* _menu;
	TeachMessageBox* _teachMsgBox;
	ThorProtocol* _broadcast;
	ThorProtocolType _broadcastType;
	SocketClient* _client;

	TyrProtocol* _protocol;
	SyncHandler* _handler;
};

