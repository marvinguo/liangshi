#pragma once

#include "../common/Menu.h"

class MainWindowMenu : public Menu
{
	Q_OBJECT
public:
	MainWindowMenu(QWidget* parent = NULL);
	~MainWindowMenu();

	virtual void InitMenu();


Q_SIGNALS:
	void SyncDevice();

private slots:
	void OnSyncDeviceTriggered();
};

