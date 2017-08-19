#include "ItemButton.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ItemButton::ItemButton(QString caption, QWidget *parent):
QPushButton(caption, parent)
{
}

ItemButton::~ItemButton()
{
}
