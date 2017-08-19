#pragma once

#include "../common/Qt.h"
#include "../../../SharedCode/LokiCommand.h"

//1 好友图标
//2 右键菜单
//3 Chat逻辑
//4 状态更新

class FriendButton : public QTreeWidgetItem
{
	friend class ChatDialog;
public:
	FriendButton(QTreeWidgetItem *parent = 0);
	~FriendButton();

	void SetFriendInfo(Loki::LokiFriend info);
	Loki::LokiFriend GetFriendInfo(){return _info;}

private:
	Qt::ItemFlags _itemFlag;
	Loki::LokiFriend _info;
};

