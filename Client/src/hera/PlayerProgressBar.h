#pragma once

#include "../common/Qt.h"
#include "SelectNumberMenu.h"

class LIANGSHIEXPORT PlayerProgressBar : public QSlider
{
	Q_OBJECT
public:
	PlayerProgressBar(QWidget *parent = 0);
	~PlayerProgressBar();

private:
	virtual void mousePressEvent(QMouseEvent * event);
};

