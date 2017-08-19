#pragma once

#include "../common/Qt.h"
#include "../../../SharedCode/LokiCommand.h"
#include "ChatDialog.h"

//chat dialog的管理由这个类完成

class FriendList : public QTreeWidget
{
	Q_OBJECT
public:
	FriendList(QWidget *parent = 0);
	~FriendList();

private Q_SLOTS:
	void OnGetFriendListTriggered(vector<Loki::LokiFriend>& friends);
	void OnItemDoubleClickTriggered(QTreeWidgetItem* item, int column);
	void OnReceiveChatTriggered(int user_id, QString& message);

private:
	ChatDialog* FindChatByUserID(int user_id);

private:
	Qt::ItemFlags _itemFlag;
	map<QString, QTreeWidgetItem*> _groups;

	//想想这个dialog,只hide,不关闭
	map<int, ChatDialog*> _chatDialogs;

	QString _defaultGroupName;
};

