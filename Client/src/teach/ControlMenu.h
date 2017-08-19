#pragma once

#include "../common/Menu.h"

class ControlMenu : public Menu
{
	Q_OBJECT
public:
	ControlMenu(QWidget* parent = NULL);
	~ControlMenu();

	virtual void InitMenu();

Q_SIGNALS:
	void OnKeSelected(QString filepath);

private Q_SLOTS:
	void OnKeSelectedTriggered();

private:
	void AddKeInDir(QString title, QString path, QtPieMenu* parent);

private:
	QString _rootFolder;
};

