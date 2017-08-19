#pragma once

#include "../common/Qt.h"
#include "OneImageButton.h"
#include "TitleButton.h"

class LIANGSHIEXPORT ScriptTitleWidget : public QWidget
{
	Q_OBJECT
public:
	ScriptTitleWidget(QWidget *parent = 0);
	~ScriptTitleWidget();

	TitleButton* GetRun(){return _run;}
	TitleButton* GetDebug(){return _debug;}
	TitleButton* GetEditor(){return _editor;}
	TitleButton* GetDeps(){return _deps;}

private Q_SLOTS:
	//void OnRunTriggered();
	//void OnDebugTriggered();

private:
	TitleButton* _run;
	TitleButton* _debug;
	TitleButton* _editor;
	TitleButton* _deps;
};

