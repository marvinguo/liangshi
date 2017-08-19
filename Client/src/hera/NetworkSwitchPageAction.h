#pragma once

#include "AnimationAction.h"

class LIANGSHIEXPORT NetworkSwitchPageAction : public AnimationAction
{
public:
	NetworkSwitchPageAction();
	~NetworkSwitchPageAction();

public:
	void Update(QString uuid, LokiBuffer& data);
	QString GetUuid();
	LokiBuffer GetPageBuffer();

public:
	virtual LokiBuffer GetMsgPackData();
	virtual AnimationType GetType();
	AnimationAction* CreateFromThis();

private:
	LokiBuffer _pageData;

public:
	MSGPACK_DEFINE(_targetScUuid, _targetElementUuid, _time, _pageData);
};
