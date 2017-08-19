#pragma once

#include "../common/Qt.h"

class ChatTextEdit : public QTextEdit
{
	Q_OBJECT

public:
	ChatTextEdit(QWidget *parent=0);
	~ChatTextEdit();

//virtuals
protected:
	virtual void keyPressEvent(QKeyEvent *event);

Q_SIGNALS:
	void OnSendMessage();
};

