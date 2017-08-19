#include "touchablescrollarea.h"
#include <QMouseEvent>
#include <QScrollBar>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif


TouchableScrollArea::TouchableScrollArea(QWidget *parent):
QScrollArea(parent)
{
//	setMouseTracking(true);
	_charm.activateOn(this);
	setAttribute(Qt::WA_AcceptTouchEvents, true);
	setWidgetResizable(true);
	//一设置这个, FlickCharm::eventFilter收不到MouseButtonPress的消息，奇怪!!!

//	viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
	//下面这句起的作用
	QScroller::grabGesture(viewport(), QScroller::LeftMouseButtonGesture);
	QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
}
	