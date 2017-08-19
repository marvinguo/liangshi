#pragma once

#include "../common/Qt.h"

class LoginTitleLabel : public QLabel
{
	Q_OBJECT
public:
	LoginTitleLabel(QString title, QWidget *parent = 0);
	virtual ~LoginTitleLabel();
};

