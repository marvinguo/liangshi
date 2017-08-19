#pragma once

#include "AnimationAction.h"

class LIANGSHIEXPORT TopMostAction : public AnimationAction
{
public:
	TopMostAction();
	~TopMostAction();

public:
	virtual LokiBuffer GetMsgPackData();
	virtual AnimationType GetType();
	AnimationAction* CreateFromThis();

public:
	MSGPACK_DEFINE(_targetScUuid, _targetElementUuid, _time);
};