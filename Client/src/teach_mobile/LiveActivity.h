#pragma once

#include "../common/Qt.h"
#include "MobileActivity.h"
#include "ActivityTitlebar.h"
#include "../hera/LiveWidget.h"

class LiveActivity : public MobileActivity
{
	Q_OBJECT
public:
	LiveActivity(MainWindow* mainWindow);
	virtual ~LiveActivity();

	void UpdateData(bool classLive, bool allTeachers, QString& session);
	virtual void OnBackKey();
	virtual void OnPause();

private Q_SLOTS:
	void OnGoBack();

private:
	ActivityTitlebar* _title;
	LiveWidget* _liveWidget;
	QVBoxLayout* _mainLayout;
	bool _classLive;
};

