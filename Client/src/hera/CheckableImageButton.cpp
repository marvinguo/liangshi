#include "CheckableImageButton.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

CheckableImageButton::CheckableImageButton(QString pic_name, QWidget *parent):
QToolButton(parent),
_isMouseovering(false),
_isMousepressing(false)
{
	QPalette text_palette = palette();
	text_palette.setColor(QPalette::ButtonText, QColor(230, 230, 230));
	setPalette(text_palette);

	QFont &text_font = const_cast<QFont &>(font());
	text_font.setWeight(QFont::Bold);

	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	QPixmap pixmap(pic_name);
	setIcon(pixmap);
	setIconSize(pixmap.size());

	setFixedSize(pixmap.width()+25, pixmap.height()+27);
	setAutoRaise(true);

	setStyleSheet("background:transparent;");

	setToolTip(tr("asdf"));
}

CheckableImageButton::~CheckableImageButton()
{
}

void CheckableImageButton::enterEvent(QEvent *)
{
	_isMouseovering = true;
}

void CheckableImageButton::leaveEvent(QEvent *)
{
	_isMouseovering = false;
}

void CheckableImageButton::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		Q_EMIT clicked();
	}
}

void CheckableImageButton::setMousePress(bool mouse_press)
{
	_isMousepressing = mouse_press;
	update();
}

void CheckableImageButton::paintEvent(QPaintEvent *event)
{
	if(_isMouseovering)
	{
		PaintInternal(0, 100, 150);
	}
	else
	{
		if(_isMousepressing)
		{
			PaintInternal(0, 100, 150);
		}
	}

	QToolButton::paintEvent(event);
}

void CheckableImageButton::PaintInternal(int top_color, int middle_color, int bottom_color)
{
	QPainter painter(this);
	QPen pen(Qt::NoBrush, 1);
	painter.setPen(pen);
	QLinearGradient linear(rect().topLeft(), rect().bottomLeft());
	linear.setColorAt(0, QColor(230, 230, 230, top_color));
	linear.setColorAt(0.5, QColor(230, 230, 230, middle_color));
	linear.setColorAt(1, QColor(230, 230, 230, bottom_color));
	painter.setBrush(linear);
	painter.drawRect(rect());
}
