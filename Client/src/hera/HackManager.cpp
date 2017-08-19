#include "HackManager.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

HackManager::HackManager():
QtSingleton<HackManager>(this)
{
#ifdef _DEBUG
	_enteredPassword = true;
#else
	_enteredPassword = false;
#endif
}

HackManager::~HackManager()
{
}

void HackManager::EnterSuperMode()
{
	bool isOK; 
	QString text = QInputDialog::getText(NULL, QObject::tr("进入超级用户模式"), QObject::tr("请输入密码:"), QLineEdit::Password, "", &isOK);
	if(isOK)
	{
		if(text == "663988")
		{
			_enteredPassword = true;
		}
	}
}

bool HackManager::IsEnableKeSuperRevise()
{
	return _enteredPassword;
}