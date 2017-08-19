#include "FriendList.h"
#include "FriendButton.h"
#include "../common/LokiClient.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

FriendList::FriendList(QWidget *parent):
QTreeWidget(parent),
_itemFlag(Qt::ItemIsEnabled | Qt::ItemIsSelectable),
_defaultGroupName(tr("我的好友"))
{
	setIndentation(8);
	header()->close();
	setStyleSheet("background-color:transparent;");
	//setAttribute(Qt::WA_TranslucentBackground,true);
	//setColumnCount(1);

	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(OnItemDoubleClickTriggered(QTreeWidgetItem*, int)));

	LokiClient& lokiClient = LokiClient::GetInstance();
	connect(&lokiClient, SIGNAL(OnGetFriendList(vector<Loki::LokiFriend>&)), this, SLOT(OnGetFriendListTriggered(vector<Loki::LokiFriend>&)));
	connect(&lokiClient, SIGNAL(OnChat(int, QString&)), this, SLOT(OnReceiveChatTriggered(int, QString&)));

	lokiClient.GetFriendList();
}

FriendList::~FriendList()
{
}

void FriendList::OnGetFriendListTriggered(vector<Loki::LokiFriend>& friends)
{
	clear();

	QTreeWidgetItem* frindGroup = new QTreeWidgetItem(this);
	frindGroup->setText(0, _defaultGroupName);
	frindGroup->setFlags(_itemFlag);
	_groups.insert(std::pair<QString, QTreeWidgetItem*>(_defaultGroupName, frindGroup));

	for(vector<Loki::LokiFriend>::iterator i = friends.begin(); i != friends.end(); ++i)
	{
		Loki::LokiFriend info = *i;
		FriendButton* aFriend = new FriendButton(frindGroup);
		//frindGroup->addChild(aFriend);
		aFriend->SetFriendInfo(info);
	}
}

ChatDialog* FriendList::FindChatByUserID(int user_id)
{
	map<int, ChatDialog*>::iterator it = _chatDialogs.find(user_id);
	if(it != _chatDialogs.end())
		return it->second;
	return NULL;
}

void FriendList::OnItemDoubleClickTriggered(QTreeWidgetItem* item, int column)
{
	FriendButton* button = dynamic_cast<FriendButton*>(item);

	if(button != NULL)
	{
		int user_id = button->GetFriendInfo().user_id;
		ChatDialog* chat = FindChatByUserID(user_id);
		
		if(chat == NULL)
		{
			chat = new ChatDialog();
			chat->SetFriendInfo(button->GetFriendInfo());
			_chatDialogs.insert(std::pair<int, ChatDialog*>(user_id, chat));
		}
		chat->show();
	}
}

void FriendList::OnReceiveChatTriggered(int user_id, QString& message)
{
	ChatDialog* chat = FindChatByUserID(user_id);

	if(chat == NULL)
	{
		chat = new ChatDialog();
		//可能需要构造一个chat user info的东西，这个会传很多，越简单，越好
		Loki::LokiFriend info;
		info.user_id = user_id;
		chat->SetFriendInfo(info);
		_chatDialogs.insert(std::pair<int, ChatDialog*>(user_id, chat));
	}
	chat->InsertReceivedMessage(message);
	chat->show();
}
