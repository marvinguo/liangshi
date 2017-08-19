#pragma once

#include "AnimationAction.h"

class LIANGSHIEXPORT LineStyleAction : public AnimationAction
{
public:
	LineStyleAction();
	~LineStyleAction();

public:
	void Update(Qt::PenStyle style);
	int GetStyle(){return _style;}

public:
	virtual LokiBuffer GetMsgPackData();
	virtual AnimationType GetType();
	AnimationAction* CreateFromThis();

private:
	int _style;

public:
	MSGPACK_DEFINE(_targetScUuid, _targetElementUuid, _time, _style);
};