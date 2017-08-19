#include "FourStateImageButton.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

FourStateImageButton::FourStateImageButton(QWidget *parent)	:
QPushButton(parent),
_status(FourStateButtonStatus_NORMAL),
_pressed(false)
{
}

FourStateImageButton::~FourStateImageButton()
{

}

void FourStateImageButton::LoadPixmap(QString pic_name)
{
	_pixmap.load(pic_name);
	_width = _pixmap.width()/4;
	_height = _pixmap.height();
	setFixedSize(_width, _height);
}

void FourStateImageButton::enterEvent(QEvent *)
{
	_status = FourStateButtonStatus_ENTER;
	update();
}

void FourStateImageButton::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		_pressed = true;
		_status = FourStateButtonStatus_PRESS;
		update();
	}
}

void FourStateImageButton::mouseReleaseEvent(QMouseEvent *)
{
	if(_pressed)
	{
		_pressed = false;
		_status = FourStateButtonStatus_ENTER;
		update();
		Q_EMIT clicked();
	}
}

void FourStateImageButton::leaveEvent(QEvent *)
{
	_status = FourStateButtonStatus_NORMAL;
	update();
}

void FourStateImageButton::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.drawPixmap(rect(), _pixmap.copy(_width * _status, 0, _width, _height));
}
