#include "SelectNumberMenu.h"
#include "../common/FileUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

SelectNumberMenu::SelectNumberMenu(QWidget* parent):
Menu(parent)
{
/*
	_rootMenu->InsertItem(tr("11"), this, SLOT(OnLineWidth()));
	_rootMenu->InsertItem(tr("12"), this, SLOT(OnLineWidth()));
	_rootMenu->InsertItem(tr("13"), this, SLOT(OnLineWidth()));
	_rootMenu->InsertItem(tr("14"), this, SLOT(OnLineWidth()));
	_rootMenu->InsertItem(tr("15"), this, SLOT(OnLineWidth()));
*/
}

SelectNumberMenu::~SelectNumberMenu()
{
}

void SelectNumberMenu::InitMenu()
{

}

void SelectNumberMenu::OnLineWidth()
{
	QtPieAction* action = dynamic_cast<QtPieAction*>(sender());
	if(action != NULL)
	{
		int width = action->GetText().toInt();
		Q_EMIT OnWidth(width);
	}
}

void SelectNumberMenu::SetNumberRange(int from, int to)
{
	_rootMenu->Clear();

	for(int i = from; i <= to; ++i)
	{
		_rootMenu->InsertItem(QString::number(i), this, SLOT(OnLineWidth()));
	}
}
