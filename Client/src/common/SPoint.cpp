#include "SPoint.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

SPoint::SPoint()
{
	_x = 0;
	_y = 0;
}

SPoint::SPoint(int x, int y)
{
	_x = x;
	_y = y;
}

SPoint::SPoint(const SPoint& p)
{
	_x = p._x; 
	_y = p._y;
}

SPoint::SPoint(const QPoint& p)
{
	_x = p.x();
	_y = p.y();
}

SPoint::SPoint(const QVector2D& p)
{
	_x = (int)(p.x());
	_y = (int)(p.y());
}

SPoint::~SPoint()
{

}

SPoint& SPoint::operator= (const QPoint &another)
{
	_x = another.x();
	_y = another.y();
	return *this;
}

SPoint& SPoint::operator= (const SPoint &another)
{
	_x = another._x;
	_y = another._y;
	return *this;
}

QPoint SPoint::ToQPoint()
{
	QPoint point;
	point.setX(_x);
	point.setY(_y);
	return point;
}
