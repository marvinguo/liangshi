#pragma once

#include "../common/Qt.h"

class ActivityTitleButton : public QPushButton
{
	Q_OBJECT
public:
	ActivityTitleButton(QString caption, QWidget *parent = 0);
	~ActivityTitleButton();
};

