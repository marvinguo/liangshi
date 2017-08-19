#pragma once

#include "../common/Qt.h"
#include "../common/YunHttp.h"
#include "MobileActivity.h"
#include "ActivityTitlebar.h"
#include "../hera/KeTime.h"
#include "../hera/KeWidget.h"

class KeActivity : public MobileActivity
{
	Q_OBJECT
public:
	KeActivity(MainWindow* mainWindow);
	virtual ~KeActivity();

	void UpdateData(QString& id);
	virtual void OnBackKey();
	virtual void OnPause();

private Q_SLOTS:
	void OnGoBack();
	void OnPreviewTirggered();

private:
	ActivityTitlebar* _title;
	KeTime* _keTime;
	KeWidget* _keWidget;
	QVBoxLayout* _mainLayout;
};

