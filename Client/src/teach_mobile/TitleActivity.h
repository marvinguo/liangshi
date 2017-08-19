#pragma once

#include "../common/Qt.h"
#include "../common/YunHttp.h"

#include "MobileActivity.h"
#include "ActivityTitlebar.h"

class TitleActivity : public MobileActivity
{
	Q_OBJECT
public:
	TitleActivity(MainWindow* mainWindow);

	virtual void OnBackKey();
	virtual void OnInitialize();
	virtual void OnPause();

Q_SIGNALS:
	void OnTeachings();
	void OnClass(ClassDTO dto);

private Q_SLOTS:
	void GetTypeTriggered(int type);
	void GetClassesTriggered(vector<ClassDTO> classes);
	void GetTeachingsTriggered(vector<TeachingDTO> teachings);
	void OnClassesClicked(QListWidgetItem *item);
	void OnTeachingsClicked(QListWidgetItem *item);
};

