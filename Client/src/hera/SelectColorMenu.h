#pragma once

#include "Menu.h"

class LIANGSHIEXPORT SelectColorMenu : public Menu
{
	Q_OBJECT
public:
	SelectColorMenu(QWidget* parent = NULL);
	virtual ~SelectColorMenu();

	virtual void InitMenu();

Q_SIGNALS:
	void OnColor(QColor color);

private Q_SLOTS:
	void OnLineColorTriggered();
};

