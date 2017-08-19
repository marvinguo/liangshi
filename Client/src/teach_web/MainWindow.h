#pragma once

#include "../common/Qt.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);

private Q_SLOTS:
	void OnCheckVersionTriggered(int version);
	void OnTitleChangedTriggered(QString title);

private:
	void InitBrowser();
};

