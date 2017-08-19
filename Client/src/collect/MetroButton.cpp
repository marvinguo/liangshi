#include "MetroButton.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

MetroButton::MetroButton(QString pic_name, QWidget *parent):
QToolButton(parent)
{
	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	//设置图标
	QPixmap pixmap(pic_name);
	setIcon(pixmap);
	setIconSize(pixmap.size());

	//设置大小
	setFixedSize(150, 190);
	setAutoRaise(true);
}

MetroButton::~MetroButton()
{
}
