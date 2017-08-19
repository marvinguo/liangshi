#include "ActivityTitleButton.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ActivityTitleButton::ActivityTitleButton(QString caption, QWidget *parent):
QPushButton(caption, parent)
{
}

ActivityTitleButton::~ActivityTitleButton()
{
}
