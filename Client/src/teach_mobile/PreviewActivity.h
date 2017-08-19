#pragma once

#include "../common/Qt.h"
#include "../common/YunHttp.h"
#include "ActivityTitlebar.h"
#include "MobileActivity.h"
#include "../hera/PlayerWidget.h"

class PreviewActivity : public MobileActivity
{
	Q_OBJECT
public:
	PreviewActivity(MainWindow* mainWindow);
	virtual ~PreviewActivity();

	void UpdateData(bool offline, QString tempFile);
	virtual void OnBackKey();
	virtual void OnPause();

private Q_SLOTS:
	void OnGoBack();

private:
	ActivityTitlebar* _title;
	KeTime* _keTime;
	PlayerWidget* _playerWidget;
	QVBoxLayout* _mainLayout;
	QVBoxLayout* _viewerLayout;

	QString _tempFile;

	bool _offlicePreview;
};

