#include "RectElement.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

RectElement::RectElement()
{
//	SetClassUuid("{5021CE15-51EC-47EF-B583-3F4851E223A4}");
}

RectElement::~RectElement()
{

}

bool RectElement::UpdatePoints(QPoint start, QPoint end)
{
	if(start == end)
		return false;

	_start = SPoint(start);
	_end = SPoint(end);
	QRect rect(Util::Point2QPoint(_start), Util::Point2QPoint(_end));
	_path = QPainterPath();
	_path.addRect(rect);
	return true;
}

ElementType RectElement::GetType()
{
	return ElementType_Rect;
}

LokiBuffer RectElement::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

void RectElement::UpdateDataFromMsgPack()
{
	PathElement::UpdateDataFromMsgPack();
	QRect rect(Util::Point2QPoint(_start), Util::Point2QPoint(_end));
	_path = QPainterPath();
	_path.addRect(rect);
}
