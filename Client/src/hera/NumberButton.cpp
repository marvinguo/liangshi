#include "NumberButton.h"
#include "../common/config.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

NumberButton::NumberButton(QString tooltip, QWidget *parent):
TitleButton(parent),
_menu(new SelectNumberMenu())
{
	//setFixedSize(22, 22);
	setFixedHeight(22);
	//setAutoRaise(true);
	setToolTip(tooltip);
	setText(QString::number(DEFAULT_LINE_WIDTH));
	_oldWidth = DEFAULT_LINE_WIDTH;

	connect(this, &NumberButton::clicked, this, &NumberButton::OnClickedTriggered);
	connect(_menu, &SelectNumberMenu::OnWidth, this, &NumberButton::OnWidthTriggered);
}

NumberButton::~NumberButton()
{
	if(_menu != NULL)
	{
		delete _menu;
		_menu = NULL;
	}
}

void NumberButton::OnClickedTriggered()
{
	_menu->Show(QCursor::pos());
}

void NumberButton::OnWidthTriggered(int width)
{
	_oldWidth = width;
	setText(QString::number(width));
	Q_EMIT OnWidthChanged(width);
}

void NumberButton::SetMiddleNumber(int number)
{
	//this->setText(QString::number(number));
	_menu->SetNumberRange(number - 4, number + 5);
}

void NumberButton::ToPenMode()
{
	SetMiddleNumber(5);
	setText(QString::number(_oldWidth));
}

void NumberButton::ToTextMode()
{

}