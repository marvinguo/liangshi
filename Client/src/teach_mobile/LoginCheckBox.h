#pragma once

#include "../common/Qt.h"

class LoginCheckBox : public QCheckBox
{
	Q_OBJECT
public:
	LoginCheckBox(QString title, QWidget *parent = 0);
	virtual ~LoginCheckBox();
};

