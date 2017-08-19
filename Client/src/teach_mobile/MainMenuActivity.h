#pragma once

#include "../common/Qt.h"
#include "../common/YunHttp.h"

#include "MobileActivity.h"
#include "ActivityTitlebar.h"

class MainMenuActivity : public MobileActivity
{
	Q_OBJECT
public:
	MainMenuActivity(MainWindow* mainWindow);

	virtual void OnBackKey();
	virtual void OnPause();

	void UpdateData();

Q_SIGNALS:
	void OnClass(ClassDTO dto);
	void OnTeaching(TeachingDTO dto);

private Q_SLOTS:
	void GetTypeTriggered(int type);
	void GetClassesTriggered(vector<ClassDTO> classes);
	void GetTeachingsTriggered(vector<TeachingDTO> teachings);
	void OnClassesClicked();
	void OnTeachingsClicked();

private:
	QVBoxLayout* _itemLayout;
	ActivityTitlebar* _title;
};

