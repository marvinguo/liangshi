#pragma once

#include "PathElement.h"

class LIANGSHIEXPORT LineElement : public PathElement
{
public:
	LineElement();
	virtual ~LineElement();

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

