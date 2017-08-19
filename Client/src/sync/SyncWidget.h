#pragma once

#include "../common/Qt.h"
#include "../common/stl.h"
#include "../common/SocketServer.h"
#include "../common/TyrProtocol.h"
#include "SyncMenu.h"

using namespace Tyr;

class SyncWidget : public QListWidget
{
	Q_OBJECT
public:
	SyncWidget(SocketClient*client, QWidget *parent = 0);
	~SyncWidget();

//QSyncWidget virtual
private:
	virtual void contextMenuEvent(QContextMenuEvent *event);
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dragMoveEvent(QDragMoveEvent *event);
	virtual void dropEvent(QDropEvent *event);

private:
	void InitMenu();
	void InitSyncWidget();

private slots:
	void OnItemDoubleClickTriggered(QListWidgetItem *item);
	void OnCommandTriggered(Tyr::TyrCommand* command);

private:
	Menu* _menu;
	QString _currentPath;
	TyrProtocol* _protocol;
};

