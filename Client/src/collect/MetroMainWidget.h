#pragma once

#include "../common/Qt.h"
#include "MetroButton.h"

class MetroMainWidget : public QWidget
{
	Q_OBJECT
public:
	MetroMainWidget(QWidget *parent = 0);
	~MetroMainWidget();

public Q_SLOTS:
	void OnShowTriggered();

private Q_SLOTS:
	void OnNewMaterialTriggered();
	void OnNewScriptMaterialTriggered();
	void OnNewTestMaterialTriggered();
	void OnNewKeTriggered();
	void OnOpenTriggered();

private:
	virtual void resizeEvent(QResizeEvent *event);
	virtual void paintEvent(QPaintEvent *e);

private:
	MetroButton* _newMaterial;
	MetroButton* _newScriptMaterial;
/*
	MetroButton* _newTest;
	MetroButton* _newMultimedia;
*/
	MetroButton* _newKe;
	MetroButton* _open;
	MetroButton* _quit;
};

