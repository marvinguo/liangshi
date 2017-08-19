#pragma once

#include "../common/Menu.h"

class SyncMenu : public Menu
{
	Q_OBJECT
public:
	SyncMenu(QWidget* parent = NULL);
	~SyncMenu();

	virtual void InitMenu();


Q_SIGNALS:
	void SyncDevice();

private slots:
	void OnSyncDeviceTriggered();
};

