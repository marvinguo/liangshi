#include "FriendButton.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

FriendButton::FriendButton(QTreeWidgetItem *parent):
QTreeWidgetItem(parent),
_itemFlag(Qt::ItemIsEnabled | Qt::ItemIsSelectable)
{
	setFlags(_itemFlag);
}

FriendButton::~FriendButton()
{

}

void FriendButton::SetFriendInfo(Loki::LokiFriend info)
{
	_info = info;

	if(!info.comment_name.empty())
	{
		setText(0, _info.comment_name.c_str());
	}
	else
	{
		setText(0, _info.display_name.c_str());
	}
}