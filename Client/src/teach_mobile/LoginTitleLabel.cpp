#include "LoginTitleLabel.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

LoginTitleLabel::LoginTitleLabel(QString title, QWidget *parent):
QLabel(title, parent)
{
}

LoginTitleLabel::~LoginTitleLabel()
{
}
