#pragma once

#include "PathElement.h"

class LIANGSHIEXPORT EllipseElement : public PathElement
{
public:
	EllipseElement();
	virtual ~EllipseElement();

	bool UpdatePoints(QPoint start, QPoint end);

//Element virtual
	virtual ElementType GetType();
	virtual LokiBuffer GetMsgPackData();
	virtual void UpdateDataFromMsgPack();

private:
	//left top, right bottom
	SPoint _start;
	SPoint _end;
public:
	MSGPACK_DEFINE(_objUuid, _lineWidth, _lineStyle, _lineColor, _matrix, _start, _end);
};

