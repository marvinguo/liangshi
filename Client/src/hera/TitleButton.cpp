#include "TitleButton.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

TitleButton::TitleButton(QWidget *parent):
QPushButton(parent)
{
}

TitleButton::TitleButton(QString caption, QWidget *parent):
QPushButton(caption, parent)
{
}

TitleButton::~TitleButton()
{
}

