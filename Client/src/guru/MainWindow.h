#pragma once

#include "../common/Qt.h"
#include "../hera/PlayerWidget.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();

private Q_SLOTS:
	void OnOpenTriggered();
	void OnConvertTriggered();

private:
	void CreateActions();
	void CreateDock();
	void CreateToolBar();

private:
	QToolBar* _toolBar;
	QAction* _openAction;
	QAction* _convertAction;

private:
	PlayerWidget* _playerWidget;
	KeTime* _keTime;
};

