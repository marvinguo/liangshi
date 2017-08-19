#include "LineElement.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

LineElement::LineElement()
{
//	SetClassUuid("{1A594596-DA8C-4DA5-AADB-C2BBFFF86BA9}");
}

LineElement::~LineElement()
{

}

bool LineElement::UpdatePoints(QPoint start, QPoint end)
{
	if(start == end)
		return false;

	_start = SPoint(start);
	_end = SPoint(end);
	_path = QPainterPath();
	_path.moveTo(Util::Point2QPoint(_start));
	_path.lineTo(Util::Point2QPoint(_end));
	return true;
}

ElementType LineElement::GetType()
{
	return ElementType_Line;
}

LokiBuffer LineElement::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

void LineElement::UpdateDataFromMsgPack()
{
	PathElement::UpdateDataFromMsgPack();
	_path = QPainterPath();
	_path.moveTo(Util::Point2QPoint(_start));
	_path.lineTo(Util::Point2QPoint(_end));
}
