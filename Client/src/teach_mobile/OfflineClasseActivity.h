#pragma once

#include "../common/Qt.h"
#include "../common/YunHttp.h"

#include "MobileActivity.h"
#include "ActivityTitlebar.h"

class OfflineClasseActivity : public MobileActivity
{
	Q_OBJECT
public:
	OfflineClasseActivity(MainWindow* mainWindow);

	void UpdateData();
	virtual void OnBackKey();
	virtual void OnPause();

private Q_SLOTS:
	void OnPlayClicked();

private:
	ActivityTitlebar* _title;
	QVBoxLayout* _viewerLayout;
};

