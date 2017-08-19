#pragma once

#include "AnimationAction.h"
#include "../common/SColor.h"

class LIANGSHIEXPORT LineColorAction : public AnimationAction
{
public:
	LineColorAction();
	~LineColorAction();

public:
	void Update(QColor color);
	SColor GetColor(){return _color;}

public:
	virtual LokiBuffer GetMsgPackData();
	virtual AnimationType GetType();
	AnimationAction* CreateFromThis();

private:
	SColor _color;

public:
	MSGPACK_DEFINE(_targetScUuid, _targetElementUuid, _time, _color);
};