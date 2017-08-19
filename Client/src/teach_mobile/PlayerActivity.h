#pragma once

#include "../common/Qt.h"
#include "../common/YunHttp.h"
#include "ActivityTitlebar.h"
#include "MobileActivity.h"
#include "../hera/PlayerWidget.h"
#include "busyindicator.h"

class PlayerActivity : public MobileActivity
{
	Q_OBJECT
public:
	PlayerActivity(MainWindow* mainWindow);
	~PlayerActivity();

	void UpdateData(bool classPlay, QString classTitle, KeDTO keDTO);
	virtual void OnBackKey();
	virtual void OnPause();

private Q_SLOTS:
	void DownloadKeFileTriggered(QString localFile);
	void OnGoBack();

private:
	static void DownloadKe(QString url, PlayerActivity* activity);

private:
	ActivityTitlebar* _title;
	KeTime* _keTime;
	QStackedLayout* _mainLayout;

	PlayerWidget* _playerWidget;
	QLabel* _busyLabel;
	QWidget* _viewer;
	QVBoxLayout* _viewerLayout;

	BusyIndicator* _busy;

	bool _classPlay;
	QString _classTitle;
	KeDTO _keDTO;
};

