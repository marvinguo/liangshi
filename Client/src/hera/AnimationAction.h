#pragma once

#include "../common/library.h"
#include "../../../SharedCode/LokiBuffer.h"

enum AnimationType
{
	AnimationType_Show = 1001,
	AnimationType_Hide = 1002,
	AnimationType_Size = 1003,
	AnimationType_LineWidth = 1004,
	AnimationType_LineStyle = 1005,
	AnimationType_LineColor = 1006,
	AnimationType_SwitchPage = 1007,
	AnimationType_TextChanged = 1008,
	AnimationType_TopMost = 1009,
	AnimationType_BottomMost = 1010,
	AnimationType_NetworkShow = 1011,
	AnimationType_NetworkSwitchPage = 1012,
};

class LIANGSHIEXPORT AnimationAction
{
public:

public:
	AnimationAction();
	virtual ~AnimationAction();
	static AnimationAction* CreateFromType(AnimationType type, LokiBuffer& buffer);

public:
	bool HasPlayed(){return _played;}
	void SetPlayed(bool play){_played = play;}
	void SetTime(int time){_time = time;}
	int GetTime(){return _time;}
	void SetTartgetBanUuid(string uuid){_targetScUuid = uuid;}
	string GetTargetScUuid(){return _targetScUuid;}
	void SetTartgetElementUuid(string uuid){_targetElementUuid = uuid;}
	string GetTargetElementUuid(){return _targetElementUuid;}

public:
	virtual LokiBuffer GetMsgPackData() = 0;
	virtual AnimationType GetType() = 0;
	virtual AnimationAction* CreateFromThis() = 0;

protected:
	int _time;
	string _targetScUuid;
	string _targetElementUuid;
	bool _played;
};



