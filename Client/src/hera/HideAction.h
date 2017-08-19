#pragma once

#include "AnimationAction.h"

class LIANGSHIEXPORT HideAction : public AnimationAction
{
public:
	HideAction();
	~HideAction();

public:
	virtual LokiBuffer GetMsgPackData();
	virtual AnimationType GetType();
	AnimationAction* CreateFromThis();

private:

public:
	MSGPACK_DEFINE(_targetScUuid, _targetElementUuid, _time);
};