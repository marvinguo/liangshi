#include "TwoStateImageButton.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

TwoStateImageButton::TwoStateImageButton(QString normalImage, QString checkedImage, QWidget *parent):
QToolButton(parent),
_isChecked(false),
_isMouseovering(false)
{
	_normal.load(normalImage);
	_checked.load(checkedImage);
	ChangeButtonImage();
	setFixedSize(22, 22);
	connect(this, &TwoStateImageButton::clicked, this, &TwoStateImageButton::OnClickTriggered);
}

TwoStateImageButton::~TwoStateImageButton()
{
}

void TwoStateImageButton::SetChecked(bool checked)
{
	if(_isChecked == checked)
		return;
	_isChecked = !_isChecked;
	ChangeButtonImage();
}

void TwoStateImageButton::ChangeButtonImage()
{
	if(_isChecked)
	{
		setIcon(_checked);
		setIconSize(_checked.size());
	}
	else
	{
		setIcon(_normal);
		setIconSize(_normal.size());
	}
}


void TwoStateImageButton::enterEvent(QEvent *)
{
	_isMouseovering = true;
	update();
}

void TwoStateImageButton::leaveEvent(QEvent *)
{
	_isMouseovering = false;
	update();
}

void TwoStateImageButton::OnClickTriggered()
{
	_isChecked = !_isChecked;
	ChangeButtonImage();
}

void TwoStateImageButton::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		Q_EMIT clicked();
	}
}

void TwoStateImageButton::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	QPixmap* pixmap;
	if(_isChecked)
		pixmap = &_checked;
	else
		pixmap = &_normal;

	painter.drawPixmap(rect(), pixmap->copy(0, 0, pixmap->width(), pixmap->height()));

	if(_isMouseovering)
	{
		PaintInternal(0, 100, 150);
	}
	QToolButton::paintEvent(event);
}

void TwoStateImageButton::PaintInternal(int top_color, int middle_color, int bottom_color)
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
