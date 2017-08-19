#pragma once

#include "PathElement.h"

class LIANGSHIEXPORT RectElement : public PathElement
{
public:
	RectElement();
	virtual ~RectElement();

	bool UpdatePoints(QPoint start, QPoint end);

//Element virtual
	virtual ElementType GetType();
	virtual LokiBuffer GetMsgPackData();
	virtual void UpdateDataFromMsgPack();

private:
	SPoint _start;
	SPoint _end;
public:
	MSGPACK_DEFINE(_objUuid, _lineWidth, _lineStyle, _lineColor, _matrix, _start, _end);
};

