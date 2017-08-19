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
	void OnLogin();
	void OnSyncCourseware();
	void OnClass();

private Q_SLOTS:
	void OnLoginTriggered();
	void OnSyncCoursewareTriggered();
	void OnClassTriggered();
};

