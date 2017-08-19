#pragma once

#include "AnimationAction.h"
#include "../common/SPoint.h"

class LIANGSHIEXPORT SizeAction : public AnimationAction
{
public:
	SizeAction();
	~SizeAction();

public:
	void Update(QPoint leftTop, int width, int height);
	SPoint GetLeftTop(){return _leftTop;}
	int GetWidth(){return _width;}
	int GetHeight(){return _height;}

public:
	virtual LokiBuffer GetMsgPackData();
	virtual AnimationType GetType();
	AnimationAction* CreateFromThis();

private:
	SPoint _leftTop;
	int _width;
	int _height;

public:
	MSGPACK_DEFINE(_targetScUuid, _targetElementUuid, _time, _leftTop, _width, _height);
};

struct ShapeAction
{
	int id;
	SizeAction* action;
};