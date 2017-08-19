#include "ElementLabelBorder.h"
#include "ImageElement.h"
#include "BanBaseWidget.h"
#include "SizeAction.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ElementLabelBorder::ElementLabelBorder(QWidget *parent, bool canResize):
QWidget(parent),
_canResize(canResize),
_titleBar(new ElementResizableTitlebar(parent)),
_isLeftPressDown(false),
_direction(ResizableElementDirection_NONE),
_borderColor(Qt::red),
_banBase(NULL)
{
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	//setAutoFillBackground(true);
	//setStyleSheet("{background-color: red}");
	//QPalette palette;
	//palette.setBrush(QPalette::Background, QBrush(Qt::red));
	//setPalette(palette);

	_label = new ElementLabel();
	QHBoxLayout* contentLayout = new QHBoxLayout();
	contentLayout->setContentsMargins(HIGHTITEM_BORDER, HIGHTITEM_BORDER, HIGHTITEM_BORDER, HIGHTITEM_BORDER);
	contentLayout->addWidget(_label);
	setLayout(contentLayout);

	ChangeParentWidget(parent);
}

ElementLabelBorder::~ElementLabelBorder()
{
	if(_titleBar != NULL)
	{
		_titleBar->deleteLater();
		_titleBar = NULL;
	}
}

void ElementLabelBorder::ChangeParentWidget(QWidget* parent)
{
	if(parent == NULL)
		return;

	if(_banBase != NULL)
	{
		disconnect(this, &ElementLabelBorder::OnDeleteElement, _banBase, &BanBaseWidget::RemoveDrawingElementTriggered);
		disconnect(this, &ElementLabelBorder::OnTopMostElement, _banBase, &BanBaseWidget::OnTopMostElementTriggered);
		disconnect(this, &ElementLabelBorder::OnBottomMostElement, _banBase, &BanBaseWidget::OnBottomMostElementTriggered);
		disconnect(this, SIGNAL(OnDirty()), _banBase, SLOT(SetModifiedTriggered()));
		disconnect(this, &ElementLabelBorder::OnAnimationAction, _banBase, &BanBaseWidget::OnAnimationAction);
		this->removeEventFilter(_banBase);
	}

	setParent(parent);
	_titleBar->setParent(parent);
	BanBaseWidget* base = dynamic_cast<BanBaseWidget*>(parent);
	if(base != NULL)
	{
		_banBase = base;
		connect(this, &ElementLabelBorder::OnDeleteElement, _banBase, &BanBaseWidget::RemoveDrawingElementTriggered);
		connect(this, &ElementLabelBorder::OnTopMostElement, _banBase, &BanBaseWidget::OnTopMostElementTriggered);
		connect(this, &ElementLabelBorder::OnBottomMostElement, _banBase, &BanBaseWidget::OnBottomMostElementTriggered);
		connect(this, SIGNAL(OnDirty()), base, SLOT(SetModifiedTriggered()));
		connect(this, &ElementLabelBorder::OnAnimationAction, _banBase, &BanBaseWidget::OnAnimationAction);
		this->installEventFilter(parent);
	}
}

void ElementLabelBorder::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	QPen pen;
	pen.setWidth(HIGHTITEM_BORDER);
	pen.setColor(_borderColor);
	painter.setPen(pen);

	QRect r = rect();
	painter.drawRect(r.topLeft().x()+1, r.topLeft().y()+1, r.width() - 1*HIGHTITEM_BORDER, r.height()-1*HIGHTITEM_BORDER);
	//painter.fillRect(rect(), QBrush(Qt::red));
}

void ElementLabelBorder::wheelEvent(QWheelEvent *e)
{
	int numDegrees = e->delta() / 8;
	int numSteps = numDegrees / 15;
	double w = width();
	double h = height();

	QDesktopWidget* desktop = QApplication::desktop();
	double screenW = desktop->width();
	double screenH = desktop->height();

	double xZoom = w / screenW;
	double yZoom = h / screenH;

	double zoom = 1;
	if(numSteps > 0)
	{
		zoom = 1.1;
	}
	else if(numSteps < 0)
	{
		zoom *= 0.9;
	}

	if(qMin(xZoom, yZoom) > MAX_ZOOM && zoom > 1)
		return;

	e->accept();
	resize(w * zoom, h * zoom);
	QWidget::wheelEvent(e);
}


void ElementLabelBorder::SetPixmap(QPixmap& pixmap, bool needResize)
{
	_label->setPixmap(pixmap);
	//_label->resize(pixmap.width(), pixmap.height());
	//下面这句话影响太大，问题太多
	if(needResize)
		resize(pixmap.width() + HIGHTITEM_BORDER*2, pixmap.height() + HIGHTITEM_BORDER*2);
}

void ElementLabelBorder::Move(int x, int y)
{
	move(x - HIGHTITEM_BORDER, y - HIGHTITEM_BORDER);
	//_label->move(x, y);
}

void ElementLabelBorder::showEvent(QShowEvent * e)
{
	_titleBar->show();
	QWidget::showEvent(e);
}

void ElementLabelBorder::hideEvent(QHideEvent * e)
{
	_titleBar->hide();
	QWidget::hideEvent(e);
}

void ElementLabelBorder::resizeEvent(QResizeEvent *e)
{
	QWidget::resizeEvent(e);
	QRect rect = _label->rect();
	SendSizeAnimation(rect.size());;
}

void ElementLabelBorder::moveEvent(QMoveEvent *e)
{
	QPoint point(pos().x(), pos().y() - 30);
	_titleBar->move(point);
	QRect rect = _label->rect();
	SendSizeAnimation(rect.size());;
	QWidget::moveEvent(e);
}

void ElementLabelBorder::enterEvent(QEvent *e)
{
	setMouseTracking(true);
	QWidget::enterEvent(e);
}

void ElementLabelBorder::leaveEvent(QEvent *e)
{
	setMouseTracking(false);
	QWidget::leaveEvent(e);
}

void ElementLabelBorder::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		_clickPoint = event->pos();
		_isLeftPressDown = true;
		if(_direction != ResizableElementDirection_NONE)
		{
			//mouseGrabber();
		}
		else
		{
			_dragPosition = event->globalPos() - frameGeometry().topLeft();
		}
		event->accept();
		//qApp->setOverrideCursor(Qt::SizeAllCursor);
	}
}

void ElementLabelBorder::mouseReleaseEvent(QMouseEvent *)
{
	_isLeftPressDown = false;
	if(_direction != ResizableElementDirection_NONE)
	{
		//releaseMouse();
		//this->setCursor(QCursor(Qt::ArrowCursor));
		qApp->restoreOverrideCursor();
	}
}

void ElementLabelBorder::mouseMoveEvent(QMouseEvent *e)
{
	QPoint gloPoint = mapToParent(e->pos());;//e->pos();
	QRect r = rect();
	QPoint tl = mapToParent(r.topLeft());
	QPoint rb = mapToParent(r.bottomRight());

	if(!_isLeftPressDown)
	{
		DetectNeedResize(gloPoint);
	}
	else
	{
		QRect rMove(tl, rb);

		switch(_direction)
		{
		case ResizableElementDirection_LEFT:
			if(rb.x() - gloPoint.x() <= this->minimumWidth())
				rMove.setX(tl.x());
			else
				rMove.setX(gloPoint.x());
			break;
		case ResizableElementDirection_RIGHT:
			rMove.setWidth(gloPoint.x() - tl.x());
			break;
		case ResizableElementDirection_UP:
			if(rb.y() - gloPoint.y() <= this->minimumHeight())
				rMove.setY(tl.y());
			else
				rMove.setY(gloPoint.y());
			break;
		case ResizableElementDirection_DOWN:
			rMove.setHeight(gloPoint.y() - tl.y());
			break;
		case ResizableElementDirection_LEFTTOP:
			if(rb.x() - gloPoint.x() <= this->minimumWidth())
				rMove.setX(tl.x());
			else
				rMove.setX(gloPoint.x());
			if(rb.y() - gloPoint.y() <= this->minimumHeight())
				rMove.setY(tl.y());
			else
				rMove.setY(gloPoint.y());
			break;
		case ResizableElementDirection_RIGHTTOP:
			rMove.setWidth(gloPoint.x() - tl.x());
			rMove.setY(gloPoint.y());
			break;
		case ResizableElementDirection_LEFTBOTTOM:
			rMove.setX(gloPoint.x());
			rMove.setHeight(gloPoint.y() - tl.y());
			break;
		case ResizableElementDirection_RIGHTBOTTOM:
			rMove.setWidth(gloPoint.x() - tl.x());
			rMove.setHeight(gloPoint.y() - tl.y());
			break;
		default:
			break;
		}
		setGeometry(rMove);
	}
	QWidget::mouseMoveEvent(e);
}

void ElementLabelBorder::DetectNeedResize(const QPoint &point)
{
	if(!_canResize)
		return;

	QRect r = rect();
	QPoint tl = mapToParent(r.topLeft());
	QPoint rb = mapToParent(r.bottomRight());
	int x = point.x();
	int y = point.y();

	//qDebug() << point << tl << rb;

	if(tl.x() + HIGHTITEM_BORDER >= x && tl.x() <= x && tl.y() + HIGHTITEM_BORDER >= y && tl.y() <= y)
	{
		_direction = ResizableElementDirection_LEFTTOP;
		this->setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if(x >= rb.x() - 20 && x <= rb.x() && y >= rb.y() - 20 && y <= rb.y())
	{
		_direction = ResizableElementDirection_RIGHTBOTTOM;
		this->setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if(x <= tl.x() + HIGHTITEM_BORDER && x >= tl.x() && y >= rb.y() - HIGHTITEM_BORDER && y <= rb.y())
	{
		_direction = ResizableElementDirection_LEFTBOTTOM;
		this->setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if(x <= rb.x() && x >= rb.x() - HIGHTITEM_BORDER && y >= tl.y() && y <= tl.y() + HIGHTITEM_BORDER)
	{
		_direction = ResizableElementDirection_RIGHTTOP;
		this->setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if(x <= tl.x() + HIGHTITEM_BORDER && x >= tl.x())
	{
		_direction = ResizableElementDirection_LEFT;
		this->setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if( x <= rb.x() && x >= rb.x() - HIGHTITEM_BORDER)
	{
		_direction = ResizableElementDirection_RIGHT;
		this->setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if(y >= tl.y() && y <= tl.y() + HIGHTITEM_BORDER)
	{
		_direction = ResizableElementDirection_UP;
		this->setCursor(QCursor(Qt::SizeVerCursor));
	}
	else if(y <= rb.y() && y >= rb.y() - HIGHTITEM_BORDER)
	{
		_direction = ResizableElementDirection_DOWN;
		this->setCursor(QCursor(Qt::SizeVerCursor));
	}
	else
	{
		_direction = ResizableElementDirection_NONE;
		//this->setCursor(QCursor(Qt::SizeAllCursor));
	}
}

void ElementLabelBorder::SendSizeAnimation(const QSize& size)
{
	Q_EMIT OnDirty();
	SizeAction action;
	action.SetTartgetBanUuid(_banBase->GetObjectUuid().toStdString());
	action.SetTartgetElementUuid(GetObjUuid());

	QRect rect = _label->rect();
	QPoint global = _label->mapToGlobal(_label->rect().topLeft());

	int x = parentWidget()->mapFromGlobal(global).x();
	int y = parentWidget()->mapFromGlobal(global).y();

	action.Update(QPoint(x, y), rect.width(), rect.height());
	Q_EMIT OnAnimationAction(action, NULL);
}