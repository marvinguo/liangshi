#include "Menu.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

//memu和pie menu都以外面的widget为生存周期
//sub menu的字会被显示出来，root menu字不会
//所以QtPieMenu的第一个参数title是空
Menu::Menu(QWidget* parent):
QObject(parent),
_rootMenu(new QtPieMenu("", parent))
{
	_rootMenu->hide();
}

Menu::~Menu()
{
	if(_rootMenu != NULL)
	{
		delete _rootMenu;
		_rootMenu = NULL;
	}
}

void Menu::Show(QPoint pos)
{
	CheckMenusBeforeShow();
	_rootMenu->Popup(pos);
}

void Menu::Hide()
{
	_rootMenu->HideMenu();
}
