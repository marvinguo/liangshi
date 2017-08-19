#include "MobileActivity.h"
#include "MainWindow.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

MobileActivity::MobileActivity(MainWindow* mainWindow, QWidget *parent):
QWidget(parent),
_mainWindow(mainWindow),
_hasInitialized(false)
{
}

MobileActivity::~MobileActivity()
{
}

#define TEST_WIDTH 400
#define TEST_HEIGHT 700

void MobileActivity::OnInitialize()
{
	int width = TEST_WIDTH;
	int height = TEST_HEIGHT;
#if defined(Q_OS_ANDROID)
	QDesktopWidget *desktop = QApplication::desktop();
	width = desktop->availableGeometry().width();
	height = desktop->availableGeometry().height();
#endif
	resize(width, height);
}

void MobileActivity::OnActive()
{
	if(!_hasInitialized)
	{
		OnInitialize();
		_hasInitialized = true;
	}
}

void MobileActivity::OnDeactive()
{

}
