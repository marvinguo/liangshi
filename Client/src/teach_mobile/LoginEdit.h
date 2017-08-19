#pragma once

#include "../common/Qt.h"

class LoginEdit : public QLineEdit
{
	Q_OBJECT
public:
	LoginEdit(QWidget *parent = 0);
	virtual ~LoginEdit();
};

