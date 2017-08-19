#pragma once

#include "AnimationAction.h"

class LIANGSHIEXPORT BottomMostAction : public AnimationAction
{
public:
	BottomMostAction();
	~BottomMostAction();

public:
	virtual LokiBuffer GetMsgPackData();
	virtual AnimationType GetType();
	AnimationAction* CreateFromThis();

public:
	MSGPACK_DEFINE(_targetScUuid, _targetElementUuid, _time);
};