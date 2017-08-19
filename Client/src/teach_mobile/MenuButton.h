#pragma once

#include "../common/Qt.h"
#include "../common/YunHttp.h"

class MenuButton : public QPushButton
{
	Q_OBJECT
public:
	MenuButton(QString caption, QWidget *parent = 0);
	~MenuButton();
};

