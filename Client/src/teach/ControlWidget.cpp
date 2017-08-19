#include "ControlWidget.h"
#include "ControlMenu.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
	#pragma execution_character_set("utf-8")
#endif

ControlWidget::ControlWidget(SocketClient* client, QWidget *parent):
QWidget(parent),
_menu(new ControlMenu(this)),
_protocol(new TyrProtocol(this)),
_backgroudPixmap(NULL)
{
	_protocol->SetSocketClient(client);

	ResizePixmap(size());
	//setAttribute(Qt::WA_AcceptsTouchEvents);

	InitMenu();
}

ControlWidget::~ControlWidget()
{

}

void ControlWidget::InitMenu()
{
	_menu->InitMenu();
	connect(_menu, SIGNAL(OnKeSelected(QString)), this, SLOT(OnKeSelectedTriggered(QString)));
}

//第一条命令
void ControlWidget::OnKeSelectedTriggered(QString filepath)
{
	//_protocol->put(filepath);
}

void ControlWidget::contextMenuEvent(QContextMenuEvent *e)
{
	_menu->Show(e->globalPos());
}

void ControlWidget::SetPainterStyle(QPainter& painter)
{
	painter.setRenderHint(QPainter::Antialiasing, true);

	/*
	QPen pen(_lineColor);
	pen.setWidth(_lineWidth);
	painter.setPen(pen);
	*/
}

void ControlWidget::ResizePixmap(const QSize& size)
{
	if(_backgroudPixmap != NULL)
		delete _backgroudPixmap;
	_backgroudPixmap = new QPixmap(size);
}

void ControlWidget::resizeEvent(QResizeEvent *e)
{
	ResizePixmap(e->size());
}

void ControlWidget::moveEvent(QMoveEvent *e)
{

}

void ControlWidget::wheelEvent(QWheelEvent *e)
{

}

void ControlWidget::paintEvent(QPaintEvent *e)
{
	_backgroudPixmap->fill(QColor(BLACKBOARD_BACKGROUND_COLOR));
	QPainter painter(this);
	SetPainterStyle(painter);

	painter.drawPixmap(0, 0, *_backgroudPixmap);
}

//handle touchevent
bool ControlWidget::event(QEvent *e)
{	
	if (e->type() == QEvent::TouchBegin)
	{
		bool accept = false;

		//	this->log->appendPlainText(QString("touch  begin: %1 point(s)").arg(static_cast<QTouchEvent *>(event)->touchPoints().count()));
		QList<QTouchEvent::TouchPoint> touches = static_cast<QTouchEvent *>(e)->touchPoints();
		int count  = touches.count();

		if(count == 2)
		{

		}

		if(count == 3)
		{
			accept = true;
			int x = (touches[0].pos().x() + touches[1].pos().x() + touches[2].pos().x()) /3;
			int y = (touches[0].pos().y() + touches[1].pos().y() + touches[2].pos().y()) /3;
			QPoint point(x, y);
			_menu->Show(point);
		}

		if(accept)
		{
			e->accept();
			return true;
		}
	}

	if (e->type() == QEvent::TouchUpdate)
	{
		int x = static_cast<QTouchEvent *>(e)->touchPoints().first().pos().x();
		int y = static_cast<QTouchEvent *>(e)->touchPoints().first().pos().y();
	//	this->log->appendPlainText(QString("touch update: %1 point(s) - (%2,%3)").arg(static_cast<QTouchEvent *>(event)->touchPoints().count()).arg(x).arg(y));
		return true;
	}

	if (e->type() == QEvent::TouchEnd) {
	//	this->log->appendPlainText(QString("touch    end: %1 point(s)").arg(static_cast<QTouchEvent *>(event)->touchPoints().count()));
		return true;
	}

	return QWidget::event(e);
}
