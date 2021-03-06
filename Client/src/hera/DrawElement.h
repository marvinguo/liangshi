#pragma once

#include "PathElement.h"

class LIANGSHIEXPORT DrawElement : public PathElement
{
public:
	DrawElement();
	virtual ~DrawElement();

	bool UpdatePoints(vector<QPoint>& points);

//Element virtual
	virtual ElementType GetType();
	virtual LokiBuffer GetMsgPackData();
	virtual void UpdateDataFromMsgPack();

private:
	vector<SPoint> _points;
public:
	MSGPACK_DEFINE(_objUuid, _lineWidth, _lineStyle, _lineColor, _matrix, _points);
};

