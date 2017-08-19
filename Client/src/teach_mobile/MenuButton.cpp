#include "MenuButton.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

MenuButton::MenuButton(QString caption, QWidget *parent):
QPushButton(caption, parent)
{
}

MenuButton::~MenuButton()
{
}
