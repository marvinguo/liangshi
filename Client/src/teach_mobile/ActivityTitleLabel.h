#pragma once

#include "../common/Qt.h"

class ActivityTitleLabel : public QLabel
{
	Q_OBJECT
public:
	ActivityTitleLabel(QString title, QWidget *parent = 0);
	virtual ~ActivityTitleLabel();

	virtual void resizeEvent(QResizeEvent *e);
};

