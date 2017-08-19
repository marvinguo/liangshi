#include "OrigialDrawElement.h"
#include "../../../SharedCode/LokiBuffer.h"
#include "../common/library.h"
#include "../common/Util.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

OrigialDrawElement::OrigialDrawElement()
{
//	SetClassUuid("{708621BF-4FD9-4F1F-84EF-6D53BBFBCC52}");
}

OrigialDrawElement::~OrigialDrawElement()
{

}

bool OrigialDrawElement::UpdatePoints(vector<QPoint>& points)
{
	_points.clear();

	for(vector<QPoint>::iterator i = points.begin(); i != points.end(); ++i)
	{
		QPoint p = *i;
		SPoint point(p);
		_points.push_back(point);
	}

	_path = ToPath(_points);
	return true;
}

ElementType OrigialDrawElement::GetType()
{
	return ElementType_OriginalDraw;
}

LokiBuffer OrigialDrawElement::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

void OrigialDrawElement::UpdateDataFromMsgPack()
{
	PathElement::UpdateDataFromMsgPack();
	_path = ToPath(_points);
}

QPainterPath OrigialDrawElement::ToPath(const vector<SPoint>& points) 
{
	QPainterPath path;

	int size = points.size();

	for(int i = 0; i < size -1; ++i)
	{
		path.moveTo(points[i]._x, points[i]._y);
		path.lineTo(points[i+1]._x, points[i+1]._y);
	}
	return path;
}