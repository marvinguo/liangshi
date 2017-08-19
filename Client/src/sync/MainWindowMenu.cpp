#include "MainWindowMenu.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
	#pragma execution_character_set("utf-8")
#endif

MainWindowMenu::MainWindowMenu(QWidget* parent):
Menu(parent)
{
}

MainWindowMenu::~MainWindowMenu()
{
}

void MainWindowMenu::InitMenu()
{
	_rootMenu->InsertItem(tr("同步设备"), this, SLOT(OnSyncDeviceTriggered()));
	//_rootMenu->InsertItem(tr("同步设备"), this, SLOT(OnSyncDeviceTriggered()));
	//_rootMenu->InsertItem(tr("同步设备"), this, SLOT(OnSyncDeviceTriggered()));
}

void MainWindowMenu::OnSyncDeviceTriggered()
{
	emit SyncDevice();
}
