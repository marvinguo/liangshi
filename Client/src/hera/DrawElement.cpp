#include "DrawElement.h"
#include "../../../SharedCode/LokiBuffer.h"
#include "../common/library.h"
#include "../common/Util.h"
#include "ToVector.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

DrawElement::DrawElement()
{
//	SetClassUuid("{708621BF-4FD9-4F1F-84EF-6D53BBFBCC52}");
}

DrawElement::~DrawElement()
{

}

bool DrawElement::UpdatePoints(vector<QPoint>& points)
{
	//这里可以设置产生point的精度
	double tolerance = 4.0;
	double error = 10;

	vector<QVector2D> generatePointVector = toVector(Util::QPoint2QVector2D(points), tolerance, error);
	if(generatePointVector.empty())
		return false;
	_points = Util::Vector2D2Point(generatePointVector);
	_path = toPath(Util::Point2Vector2D(_points));
	return true;
}

ElementType DrawElement::GetType()
{
	return ElementType_Draw;
}

LokiBuffer DrawElement::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

void DrawElement::UpdateDataFromMsgPack()
{
	PathElement::UpdateDataFromMsgPack();
	_path = toPath(Util::Point2Vector2D(_points));
}