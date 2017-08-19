#pragma once

#include "PathElement.h"

class LIANGSHIEXPORT OrigialDrawElement : public PathElement
{
public:
	OrigialDrawElement();
	virtual ~OrigialDrawElement();

	bool UpdatePoints(vector<QPoint>& points);

//Element virtual
	virtual ElementType GetType();
	virtual LokiBuffer GetMsgPackData();
	virtual void UpdateDataFromMsgPack();

private:
	QPainterPath ToPath(const vector<SPoint>& inputPoints);

private:
	vector<SPoint> _points;
public:
	MSGPACK_DEFINE(_objUuid, _lineWidth, _lineStyle, _lineColor, _matrix, _points);
};

