#pragma once

#include "../common/Qt.h"

class HeaderLabel : public QLabel
{
	Q_OBJECT
public:
	HeaderLabel(QString title, QWidget *parent = 0);
	virtual ~HeaderLabel();
};

