#pragma once

#include "../common/Qt.h"
#include "../common/library.h"

class LIANGSHIEXPORT TitleButton : public QPushButton
{
	Q_OBJECT
public:
	TitleButton(QWidget *parent = 0);
	TitleButton(QString caption, QWidget *parent = 0);
	~TitleButton();
};

