#include "SelectColorMenu.h"
#include "../common/FileUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

SelectColorMenu::SelectColorMenu(QWidget* parent):
Menu(parent)
{
	_rootMenu->InsertItem(tr("#000000"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#FF33CC"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#CC0099"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#9900FF"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#800080"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#6600FF"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#00FFFF"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#3399CC"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#0033CC"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#003366"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#000099"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#66FF66"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#00FF99"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#00FF00"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#009900"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#336600"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#FFFF99"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#FFCC66"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#FFCC00"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#FFFF00"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#FF5E5E"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#FF0000"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#FF0066"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#FF33CC"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#663300"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#CC3300"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#A50021"), this, SLOT(OnLineColorTriggered()));
	_rootMenu->InsertItem(tr("#ffffff"), this, SLOT(OnLineColorTriggered()));
}

SelectColorMenu::~SelectColorMenu()
{
}

void SelectColorMenu::InitMenu()
{

}

void SelectColorMenu::OnLineColorTriggered()
{
	QtPieAction* action = dynamic_cast<QtPieAction*>(sender());
	if(action != NULL)
	{
		QColor color(action->GetText());
		Q_EMIT OnColor(color);
	}
}