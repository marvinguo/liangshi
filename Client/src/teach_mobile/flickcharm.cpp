/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Graphics Dojo project on Qt Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "flickcharm.h"

#include <QtWidgets>
#include <QDebug>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif


struct FlickData {
	typedef enum { Steady, Pressed, ManualScroll, AutoScroll, Stop } State;
	State state;
	QWidget *widget;
	QPoint pressPos;
	QPoint offset;
	QPoint dragPos;
	QPoint speed;
	QList<QEvent*> ignored;
};

class FlickCharmPrivate
{
public:
	FlickCharmPrivate();
	~FlickCharmPrivate();

	QHash<QWidget*, FlickData*> flickData;
	QBasicTimer ticker;
};

FlickCharmPrivate::FlickCharmPrivate()
{

}

FlickCharmPrivate::~FlickCharmPrivate()
{
	foreach (FlickData* flick, flickData)
	{
		delete flick;
	}
	flickData.clear();
}

FlickCharm::FlickCharm(QObject *parent): QObject(parent)
{
	d = new FlickCharmPrivate;
}

FlickCharm::~FlickCharm()
{
	delete d;
}

void FlickCharm::activateOn(QWidget *widget)
{
	QAbstractScrollArea *scrollArea = dynamic_cast<QAbstractScrollArea*>(widget);
	if (scrollArea)
	{

//		scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//		scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

		scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

		QWidget *viewport = scrollArea->viewport();

		viewport->installEventFilter(this);
		scrollArea->installEventFilter(this);

		d->flickData.remove(viewport);
		d->flickData[viewport] = new FlickData;
		d->flickData[viewport]->widget = widget;
		d->flickData[viewport]->state = FlickData::Steady;

		return;
	}

	qWarning() << "FlickCharm only works on QAbstractScrollArea (and derived classes)";
	qWarning() << "or QWebView (and derived classes)";
}

void FlickCharm::deactivateFrom(QWidget *widget)
{
	QAbstractScrollArea *scrollArea = dynamic_cast<QAbstractScrollArea*>(widget);
	if (scrollArea) {
		QWidget *viewport = scrollArea->viewport();

		viewport->removeEventFilter(this);
		scrollArea->removeEventFilter(this);

		delete d->flickData[viewport];
		d->flickData.remove(viewport);

		return;
	}
}

static QPoint scrollOffset(QWidget *widget)
{
	int x = 0, y = 0;

	QAbstractScrollArea *scrollArea = dynamic_cast<QAbstractScrollArea*>(widget);
	if (scrollArea) {
		x = scrollArea->horizontalScrollBar()->value();
		y = scrollArea->verticalScrollBar()->value();
	}

	return QPoint(x, y);
}

static void setScrollOffset(QWidget *widget, const QPoint &p)
{
	QAbstractScrollArea *scrollArea = dynamic_cast<QAbstractScrollArea*>(widget);
	if (scrollArea) {
		scrollArea->horizontalScrollBar()->setValue(p.x());
		scrollArea->verticalScrollBar()->setValue(p.y());
	}
}

static QPoint deaccelerate(const QPoint &speed, int a = 1, int max = 64)
{
	int x = qBound(-max, speed.x(), max);
	int y = qBound(-max, speed.y(), max);
	x = (x == 0) ? x : (x > 0) ? qMax(0, x - a) : qMin(0, x + a);
	y = (y == 0) ? y : (y > 0) ? qMax(0, y - a) : qMin(0, y + a);
	return QPoint(x, y);
}

bool FlickCharm::eventFilter(QObject *object, QEvent *event)
{
	if (!object->isWidgetType())
		return false;

	QEvent::Type type = event->type();
	if(type != QEvent::MouseButtonPress && type != QEvent::MouseButtonRelease && type != QEvent::MouseMove)
		return false;

	QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent*>(event);
	if(!mouseEvent || mouseEvent->modifiers() != Qt::NoModifier)
		return false;

	QWidget *viewport = dynamic_cast<QWidget*>(object);
	FlickData *data = d->flickData.value(viewport);
	if (!viewport || !data || data->ignored.removeAll(event))
		return false;

	bool consumed = false;
	switch (data->state)
	{
	case FlickData::Steady:
		//setWidgetResizable(true);以后不工作， 不知为什么
		if(type == QEvent::MouseButtonPress)
		{
			if (mouseEvent->buttons() == Qt::LeftButton)
			{
				consumed = true;
				data->state = FlickData::Pressed;
				data->pressPos = mouseEvent->pos();
				data->offset = scrollOffset(data->widget);
				qDebug() << "Steady works";
			}
		}
		break;

	case FlickData::Pressed:
		if(type == QEvent::MouseButtonRelease)
		{
			consumed = true;
			data->state = FlickData::Steady;

			QMouseEvent *event1 = new QMouseEvent(QEvent::MouseButtonPress,
				data->pressPos, Qt::LeftButton,
				Qt::LeftButton, Qt::NoModifier);
			QMouseEvent *event2 = new QMouseEvent(*mouseEvent);

			data->ignored << event1;
			data->ignored << event2;
			QApplication::postEvent(object, event1);
			QApplication::postEvent(object, event2);
			qDebug() << "Pressed  MouseButtonRelease works";
		}
		if (mouseEvent->type() == QEvent::MouseMove)
		{
			consumed = true;
			data->state = FlickData::ManualScroll;
			data->dragPos = QCursor::pos();
			if (!d->ticker.isActive())
				d->ticker.start(20, this);
			qDebug() << "Pressed MouseMove works";
		}
		break;

	case FlickData::ManualScroll:
		if(type == QEvent::MouseMove)
		{
			consumed = true;
			QPoint delta = mouseEvent->pos() - data->pressPos;
			setScrollOffset(data->widget, data->offset - delta);
			qDebug() << "ManualScroll MouseMove works";
		}
		if(type == QEvent::MouseButtonRelease)
		{
			consumed = true;
			data->state = FlickData::AutoScroll;
			qDebug() << "ManualScroll MouseButtonRelease works";
		}
		break;

	case FlickData::AutoScroll:
		if(type == QEvent::MouseButtonPress)
		{
			consumed = true;
			data->state = FlickData::Stop;
			data->speed = QPoint(0, 0);
			data->pressPos = mouseEvent->pos();
			data->offset = scrollOffset(data->widget);
			qDebug() << "AutoScroll MouseButtonPress works";
		}
		if(type == QEvent::MouseButtonRelease)
		{
			consumed = true;
			data->state = FlickData::Steady;
			data->speed = QPoint(0, 0);
			qDebug() << "AutoScroll MouseButtonRelease works";
		}
		break;

	case FlickData::Stop:
		if(type == QEvent::MouseButtonRelease)
		{
			consumed = true;
			data->state = FlickData::Steady;
			qDebug() << "Stop MouseButtonRelease works";
		}
		if(type == QEvent::MouseMove)
		{
			consumed = true;
			data->state = FlickData::ManualScroll;
			data->dragPos = QCursor::pos();
			if (!d->ticker.isActive())
				d->ticker.start(20, this);
			qDebug() << "Stop MouseMove works";
		}
		break;

	default:
		break;
	}
	return consumed;
}

void FlickCharm::timerEvent(QTimerEvent *event)
{
	int count = 0;
	QHashIterator<QWidget*, FlickData*> item(d->flickData);
	while (item.hasNext()) {
		item.next();
		FlickData *data = item.value();

		if (data->state == FlickData::ManualScroll) {
			count++;
			data->speed = QCursor::pos() - data->dragPos;
			data->dragPos = QCursor::pos();
		}

		if (data->state == FlickData::AutoScroll) {
			count++;
			data->speed = deaccelerate(data->speed);
			QPoint p = scrollOffset(data->widget);
			setScrollOffset(data->widget, p - data->speed);
			if (data->speed == QPoint(0, 0))
				data->state = FlickData::Steady;
		}
	}

	if (!count)
		d->ticker.stop();

	QObject::timerEvent(event);
}
