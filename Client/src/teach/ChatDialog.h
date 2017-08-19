#pragma once

#include "../common/Qt.h"
#include "ui_ChatDialog.h"
#include "FriendButton.h"

struct ChatFile
{
	QFont font;
	QColor color;
	friend QDataStream & operator<< (QDataStream &qos, const ChatFile &sf)
	{
		qos << sf.font << sf.color;
		return qos;
	}
	friend QDataStream & operator>> (QDataStream &qis, ChatFile &sf)
	{
		qis >> sf.font >> sf.color;
		return qis;
	}
};

class ChatDialog : public QDialog
{
	Q_OBJECT
public:
	ChatDialog(QWidget *parent=0);
	~ChatDialog();
	void SetFriendInfo(Loki::LokiFriend info){_info = info;}
	void InsertReceivedMessage(QString& message);

private Q_SLOTS:
	void OnColorTriggered();
	void OnFontTriggered();
	void OnSendTriggered();
	void OnMaterialTriggered();
	void InputTextChangeTriggered();

private:
	void SaveChatFile();
	void OpenChatFile();

private:
	Ui::ChatDialog _ui;
	Loki::LokiFriend _info;
	ChatFile _chatFile;
};

