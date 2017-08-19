#include "MainWindowMenu.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
	#pragma execution_character_set("utf-8")
#endif

MainWindowMenu::MainWindowMenu(QWidget* parent):
Menu(parent)
{
}

MainWindowMenu::~MainWindowMenu()
{
}

void MainWindowMenu::InitMenu()
{
	_rootMenu->InsertItem(tr("登录"), this, SLOT(OnLoginTriggered()));
	_rootMenu->InsertItem(tr("同步课件"), this, SLOT(OnSyncCoursewareTriggered()));
	_rootMenu->InsertItem(tr("上课"), this, SLOT(OnClassTriggered()));

		QtPieMenu *penSubMenu = new QtPieMenu("画笔", _rootMenu, "");
	penSubMenu->InsertItem(tr("粉笔"), this, SLOT(OnPen()));
	penSubMenu->InsertItem(tr("直线"), this, SLOT(OnLine()));
	penSubMenu->InsertItem(tr("矩形"), this, SLOT(OnRectangle()));
	penSubMenu->InsertItem(tr("正方形"), this, SLOT(OnPerfectRectangleTriggered()));
	penSubMenu->InsertItem(tr("椭圆"), this, SLOT(OnEllipse()));
	penSubMenu->InsertItem(tr("正圆"), this, SLOT(OnPerfectEllipseTriggered()));
	penSubMenu->InsertItem(tr("撤销"), this, SLOT(OnUndo()));
	penSubMenu->InsertItem(tr("重做"), this, SLOT(OnRedo()));
	_rootMenu->InsertItem(penSubMenu);
}

void MainWindowMenu::OnLoginTriggered()
{
	emit OnLogin();
}

void MainWindowMenu::OnSyncCoursewareTriggered()
{
	emit OnSyncCourseware();
}

void MainWindowMenu::OnClassTriggered()
{
	emit OnClass();
}
