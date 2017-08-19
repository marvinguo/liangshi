#include "ActivityTitleLabel.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ActivityTitleLabel::ActivityTitleLabel(QString title, QWidget *parent):
QLabel(title, parent)
{
	setScaledContents(true);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

ActivityTitleLabel::~ActivityTitleLabel()
{
}

void ActivityTitleLabel::resizeEvent(QResizeEvent *e)
{
	return QLabel::resizeEvent(e);
}
