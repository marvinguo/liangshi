#pragma once
#include "../common/library.h"
#include "CameraWindow.h"
#include "HackManager.h"
#include "HeraDocManager.h"
#include "HtmlElementEditor.h"
#include "AddCorrectDialog.h"
#include "BanBrowserDialog.h"
#include "KePages.h"
#ifdef _LiangshiDesktop
#include "NotMobile/BanScriptDepsDialog.h"
#endif
#include "ScreenshotWindow.h"
#include "NotifyWindow.h"
#include "SlidePresenterManager.h"

class LIANGSHIEXPORT HeraSingletons
{
public:
	HeraSingletons();
	virtual ~HeraSingletons();

private:
	AddCorrectDialog _addCorrect;
#ifdef _LiangshiDesktop
	BanScriptDepsDialog _deps;
#endif
	BanBrowserDialog _browser;
#ifdef Q_OS_WIN32
	CameraWindow _camera;
#endif
	HackManager _hack;
	HeraDocManager _manager;
	HtmlElementEditor _editor;
	KePages _pages;
	ScreenshotWindow _screen;
	NotifyWindow _notify;
	SlidePresenterManager _slideManager;
};
