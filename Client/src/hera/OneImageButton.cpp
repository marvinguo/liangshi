#include "OneImageButton.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

OneImageButton::OneImageButton(QString path, QString tooltip, QWidget *parent):
QToolButton(parent),
_isMouseovering(false)
{
	QPalette text_palette = palette();
	text_palette.setColor(QPalette::ButtonText, QColor(230, 230, 230));
	setPalette(text_palette);

	QPixmap pixmap(path);
	setIcon(pixmap);
	setIconSize(pixmap.size());

	setFixedSize(22, 22);
	setAutoRaise(true);

	//setStyleSheet("background:transparent;");
	setToolTip(tooltip);
}

OneImageButton::~OneImageButton()
{
}

void OneImageButton::enterEvent(QEvent *)
{
	_isMouseovering = true;
	update();
}

void OneImageButton::leaveEvent(QEvent *)
{
	_isMouseovering = false;
	update();
}

void OneImageButton::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		Q_EMIT clicked();
	}
}

void OneImageButton::paintEvent(QPaintEvent *event)
{
	if(_isMouseovering)
	{
		PaintInternal(0, 100, 150);
	}
	QToolButton::paintEvent(event);
}

void OneImageButton::PaintInternal(int top_color, int middle_color, int bottom_color)
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
