#pragma once

#include "AnimationAction.h"

class LIANGSHIEXPORT SwitchPageAction : public AnimationAction
{
public:
	SwitchPageAction();
	~SwitchPageAction();

public:
	virtual LokiBuffer GetMsgPackData();
	virtual AnimationType GetType();
	AnimationAction* CreateFromThis();

private:

public:
	MSGPACK_DEFINE(_targetScUuid, _targetElementUuid, _time);
};
