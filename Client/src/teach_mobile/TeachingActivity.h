#pragma once

#include "../common/Qt.h"
#include "../common/YunHttp.h"

#include "MobileActivity.h"
#include "ActivityTitlebar.h"

class TeachingActivity : public MobileActivity
{
	Q_OBJECT
public:
	TeachingActivity(MainWindow* mainWindow);

	void UpdateData(TeachingDTO dto);
	virtual void OnBackKey();
	virtual void OnPause();

Q_SIGNALS:
	void OnCollect(QString id);

private Q_SLOTS:
	void GetKesTriggered(vector<KeDTO> kes);
	void OnPlayClicked();
	void OnCollectClicked();
	void OnLiveClicked();
	void OnDownlaodALLClicked();

private:
	ActivityTitlebar* _title;
	TeachingDTO _teachingDTO;
	vector<KeDTO> _kes;
	QHBoxLayout* _firstLayout;
	QVBoxLayout* _viewerLayout;

	QVBoxLayout* _classLayout;
	QVBoxLayout* _questionLayout;
};

