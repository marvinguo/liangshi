#pragma once

#include "../common/Qt.h"
#include "SelectColorMenu.h"
#include "TitleButton.h"

class LIANGSHIEXPORT ColorButton : public TitleButton
{
	Q_OBJECT
public:
	ColorButton(QString tooltip, QWidget *parent = 0);
	~ColorButton();

Q_SIGNALS:
	void OnColorChanged(QColor color);

private Q_SLOTS:
	void OnClickedTriggered();
	void OnColorTriggered(QColor color);

private:
	void SetBackgroundColor(QColor color);

private:
	SelectColorMenu* _menu;
};

