#include "KeItemButton.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

KeItemButton::KeItemButton(QString caption, QWidget *parent):
QPushButton(caption, parent)
{
}

KeItemButton::~KeItemButton()
{
}
