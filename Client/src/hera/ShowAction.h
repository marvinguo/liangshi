#pragma once

#include "AnimationAction.h"

class LIANGSHIEXPORT ShowAction : public AnimationAction
{
public:
	ShowAction();
	~ShowAction();

public:
	virtual LokiBuffer GetMsgPackData();
	virtual AnimationType GetType();
	AnimationAction* CreateFromThis();

private:

public:
	MSGPACK_DEFINE(_targetScUuid, _targetElementUuid, _time);
};
