#pragma once
#include "../common/Qt.h"
#include "../common/library.h"

class MetroButton : public QToolButton
{
	Q_OBJECT
public:
	MetroButton(QString pic_name, QWidget *parent = 0);
	~MetroButton();
};
