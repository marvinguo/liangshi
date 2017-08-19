#include "EllipseElement.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

EllipseElement::EllipseElement()
{
//	SetClassUuid("{CA27A722-D837-4447-B15F-9BCB753ECC66}");
}

EllipseElement::~EllipseElement()
{
}

bool EllipseElement::UpdatePoints(QPoint start, QPoint end)
{
	if(start == end)
		return false;

	_start = SPoint(start);
	_end = SPoint(end);
	
	QRect rect(Util::Point2QPoint(_start), Util::Point2QPoint(_end));
	_path = QPainterPath();
	_path.addEllipse(rect);
	return true;
}

ElementType EllipseElement::GetType()
{
	return ElementType_Ellipse;
}

LokiBuffer EllipseElement::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

void EllipseElement::UpdateDataFromMsgPack()
{
	PathElement::UpdateDataFromMsgPack();
	QRect rect(Util::Point2QPoint(_start), Util::Point2QPoint(_end));
	_path = QPainterPath();
	_path.addEllipse(rect);
}