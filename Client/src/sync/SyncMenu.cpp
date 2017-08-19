#include "SyncMenu.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
	#pragma execution_character_set("utf-8")
#endif

SyncMenu::SyncMenu(QWidget* parent):
Menu(parent)
{
}

SyncMenu::~SyncMenu()
{
}

void SyncMenu::InitMenu()
{
	_rootMenu->InsertItem(tr("同步设备"), this, SLOT(OnSyncDeviceTriggered()));
	//_rootMenu->InsertItem(tr("同步设备"), this, SLOT(OnSyncDeviceTriggered()));
	//_rootMenu->InsertItem(tr("同步设备"), this, SLOT(OnSyncDeviceTriggered()));
}

void SyncMenu::OnSyncDeviceTriggered()
{
	emit SyncDevice();
}
