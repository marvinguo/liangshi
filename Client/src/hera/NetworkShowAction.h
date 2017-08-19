#pragma once

#include "AnimationAction.h"

class LIANGSHIEXPORT NetworkShowAction : public AnimationAction
{
public:
	NetworkShowAction();
	~NetworkShowAction();

public:
	void Update(int type, LokiBuffer& data);
	int GetElementType();
	LokiBuffer GetElementBuffer();

public:
	virtual LokiBuffer GetMsgPackData();
	virtual AnimationType GetType();
	AnimationAction* CreateFromThis();

private:
	int _elementType;
	LokiBuffer _elementData;

public:
	MSGPACK_DEFINE(_targetScUuid, _targetElementUuid, _time, _elementType, _elementData);
};
