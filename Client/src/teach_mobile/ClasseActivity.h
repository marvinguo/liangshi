#pragma once

#include "../common/Qt.h"
#include "../common/YunHttp.h"

#include "MobileActivity.h"
#include "ActivityTitlebar.h"

class ClasseActivity : public MobileActivity
{
	Q_OBJECT
public:
	ClasseActivity(MainWindow* mainWindow);

	void UpdateData(ClassDTO dto);
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
	ClassDTO _classDTO;
	vector<KeDTO> _kes;

	QHBoxLayout* _firstLayout;
	QVBoxLayout* _viewerLayout;

	QVBoxLayout* _classLayout;
	QVBoxLayout* _questionLayout;
};

