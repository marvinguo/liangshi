#pragma once

#include "AnimationAction.h"

class LIANGSHIEXPORT LineWidthAction : public AnimationAction
{
public:
	LineWidthAction();
	~LineWidthAction();

public:
	void Update(int width);
	int GetWidth(){return _width;}

public:
	virtual LokiBuffer GetMsgPackData();
	virtual AnimationType GetType();
	AnimationAction* CreateFromThis();

private:
	int _width;

public:
	MSGPACK_DEFINE(_targetScUuid, _targetElementUuid, _time, _width);
};