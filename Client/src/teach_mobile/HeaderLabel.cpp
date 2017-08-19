#include "HeaderLabel.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

HeaderLabel::HeaderLabel(QString title, QWidget *parent):
QLabel(title, parent)
{
}

HeaderLabel::~HeaderLabel()
{
}
