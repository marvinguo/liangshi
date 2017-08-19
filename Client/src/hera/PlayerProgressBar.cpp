#include "PlayerProgressBar.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

PlayerProgressBar::PlayerProgressBar(QWidget *parent):
QSlider(Qt::Horizontal, parent)
{
	//setStyle(QStyleFactory::create("android"));
}

PlayerProgressBar::~PlayerProgressBar()
{
}

void PlayerProgressBar::mousePressEvent(QMouseEvent * event)
{
#ifdef Q_OS_ANDROID
	if (event->button() == Qt::LeftButton)
	{
		if (orientation() == Qt::Vertical)
			setValue(minimum() + ((maximum()-minimum()) * (height()-event->y())) / height() ) ;
		else
			setValue(minimum() + ((maximum()-minimum()) * event->x()) / width() ) ;

		event->accept();
	}
#endif
	QSlider::mousePressEvent(event);
}