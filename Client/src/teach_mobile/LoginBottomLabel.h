#pragma once

#include "../common/Qt.h"

class LoginBottomLabel : public QLabel
{
	Q_OBJECT
public:
	LoginBottomLabel(QString title, QWidget *parent = 0);
	virtual ~LoginBottomLabel();
};

