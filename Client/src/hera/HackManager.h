#pragma once

#include "../common/Qt.h"
#include "../common/library.h"
#include "../common/QtSingleton.h"

class LIANGSHIEXPORT HackManager : public QtSingleton<HackManager>
{
public:
	HackManager();
	virtual ~HackManager();

	void EnterSuperMode();
	bool IsEnableKeSuperRevise();

private:
	bool _enteredPassword;
};

//好像从dll出去的singleton,有问题，不能实例
//typedef CSingleton<HackManager> HackManagerSingleton;
