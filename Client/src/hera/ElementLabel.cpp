#include "ElementLabel.h"
#include "ImageElement.h"
#include "BanBaseWidget.h"
#include "ElementLabelBorder.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ElementLabel::ElementLabel(QWidget *parent):
QLabel(parent)
{
	//setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setScaledContents(true);
	//setFrameStyle(QFrame::NoFrame);
}

ElementLabel::~ElementLabel()
{
}

void ElementLabel::wheelEvent(QWheelEvent *e)
{
	ElementLabelBorder* border = qobject_cast<ElementLabelBorder*>(parentWidget());
	if(border != NULL)
		border->wheelEvent(e);
	//QLabel::wheelEvent(e);
}

void ElementLabel::enterEvent(QEvent *e)
{
	parentWidget()->setMouseTracking(false);
	setCursor(QCursor(Qt::SizeAllCursor));
	QLabel::enterEvent(e);
}

void ElementLabel::leaveEvent(QEvent *e)
{
	parentWidget()->setMouseTracking(true);
	QLabel::leaveEvent(e);
}

void ElementLabel::mousePressEvent(QMouseEvent *e)
{
	_clickPoint = e->pos();
	_isMoving = true;
	QLabel::mousePressEvent(e);
}

void ElementLabel::mouseReleaseEvent(QMouseEvent *e)
{
	if(_isMoving)
	{
		_isMoving = false;
	}
	QLabel::mouseReleaseEvent(e);
}

void ElementLabel::mouseMoveEvent(QMouseEvent *e)
{
	if((e->buttons() == Qt::LeftButton) && _isMoving)
	{
		QWidget* parent_widget = this->parentWidget();
		QPoint parent_point = parent_widget->pos();
		parent_point.setX(parent_point.x() + e->x() - _clickPoint.x());
		parent_point.setY(parent_point.y() + e->y() - _clickPoint.y());
		parent_widget->move(parent_point);
	}
	//QLabel::mouseMoveEvent(e);
}
