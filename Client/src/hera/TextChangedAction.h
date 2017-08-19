#pragma once

#include "AnimationAction.h"

class LIANGSHIEXPORT TextChangedAction : public AnimationAction
{
public:
	TextChangedAction();
	~TextChangedAction();

public:
	void Update(QString text, int fontSize);
	string GetText(){return _text;}
	int GetFontSize(){return _fontSize;}

public:
	virtual LokiBuffer GetMsgPackData();
	virtual AnimationType GetType();
	AnimationAction* CreateFromThis();

private:
	string _text;
	int _fontSize;

public:
	MSGPACK_DEFINE(_targetScUuid, _targetElementUuid, _time, _text, _fontSize);
};
