#pragma once

#include "Menu.h"

class LIANGSHIEXPORT SelectNumberMenu : public Menu
{
	Q_OBJECT
public:
	SelectNumberMenu(QWidget* parent = NULL);
	virtual ~SelectNumberMenu();

	virtual void InitMenu();

	void SetNumberRange(int from, int to);

Q_SIGNALS:
	void OnWidth(int width);

private Q_SLOTS:
	void OnLineWidth();
};

