#include "ColorButton.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ColorButton::ColorButton(QString tooltip, QWidget *parent):
TitleButton(parent),
_menu(new SelectColorMenu())
{
	//setFixedSize(22, 22);
#ifdef _LiangshiDesktop
	setFixedHeight(22);
#endif
	//setAutoRaise(true);
	setToolTip(tooltip);
	SetBackgroundColor(Qt::white);

	connect(this, &ColorButton::clicked, this, &ColorButton::OnClickedTriggered);
	connect(_menu, &SelectColorMenu::OnColor, this, &ColorButton::OnColorTriggered);
}

ColorButton::~ColorButton()
{
	if(_menu != NULL)
	{
		delete _menu;
		_menu = NULL;
	}
}

void ColorButton::OnClickedTriggered()
{
	_menu->Show(QCursor::pos());
}

void ColorButton::OnColorTriggered(QColor color)
{
	SetBackgroundColor(color);
	Q_EMIT OnColorChanged(color);
}

void ColorButton::SetBackgroundColor(QColor color)
{
/*
	QPalette palette1(palette());
	//palette.setColor(backgroundRole(), color);
	palette1.setColor(QPalette::Background, color);
	this->setAutoFillBackground(true);
	this->setPalette(palette1);
*/
	//setStyleSheet ("background-color: rgb(0,0,0);color: rgb(255,255,255);");
	QString style = "ColorButton {background-color:"+ color.name()+ ";}";
	setStyleSheet(style);
}

