#pragma once

#include "../common/Qt.h"
#include "../hera/KeWidget.h"

#include "ActionTable.h"
#include "BanTable.h"
#include "ElementTable.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();

private Q_SLOTS:
	void OnOpenTriggered();
	void OnSaveTriggered();

private:
	void CreateActions();
	void CreateDock();
	void CreateToolBar();

private:
	QToolBar* _toolBar;
	QAction* _openAction;
	QAction* _saveAction;

	ActionTable* _actionTable;
	BanTable* _banTable;
	ElementTable* _elementTable;
private:
	KeWidget* _keWidget;
	KeTime* _keTime;
};

