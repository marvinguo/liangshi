#pragma once

#include "../common/Qt.h"

class LoginButton : public QPushButton
{
	Q_OBJECT
public:
	LoginButton(QString title, QWidget *parent = 0);
	virtual ~LoginButton();
};

