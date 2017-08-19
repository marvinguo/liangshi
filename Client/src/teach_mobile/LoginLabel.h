#pragma once

#include "../common/Qt.h"

class LoginLabel : public QLabel
{
	Q_OBJECT
public:
	LoginLabel(QString title, QWidget *parent = 0);
	virtual ~LoginLabel();
};

