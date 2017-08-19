#pragma once

#include "../common/Qt.h"
#include "../common/library.h"
#include "qtpiemenu.h"
#include "qtpieaction.h"

class Menu : public QObject
{
	Q_OBJECT
public:
	Menu(QWidget* parent = NULL);
	virtual ~Menu();

	virtual void InitMenu() = 0;
	virtual void CheckMenusBeforeShow(){}

	void Show(QPoint pos);
	void Hide();
	QWidget* GetPieMenuWidget(){return _rootMenu;}

protected:
	QtPieMenu* _rootMenu;
};

