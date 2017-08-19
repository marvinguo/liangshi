#pragma once

#include "../common/Qt.h"
#include "SelectNumberMenu.h"
#include "TitleButton.h"

class LIANGSHIEXPORT NumberButton : public TitleButton
{
	Q_OBJECT
public:
	NumberButton(QString tooltip, QWidget *parent = 0);
	~NumberButton();

	void SetMiddleNumber(int number);
	void ToPenMode();
	void ToTextMode();

Q_SIGNALS:
	void OnWidthChanged(int width);

private Q_SLOTS:
	void OnClickedTriggered();
	void OnWidthTriggered(int width);

private:
	SelectNumberMenu* _menu;
	int _oldWidth;
};

