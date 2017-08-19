#include "ChatTextEdit.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ChatTextEdit::ChatTextEdit(QWidget *parent/* =0 */)
: QTextEdit(parent)
{

}

ChatTextEdit::~ChatTextEdit()
{

}

void ChatTextEdit::keyPressEvent(QKeyEvent *event)
{
	//如果按下Ctrl+Enter，就发送信息，否则就按默认的方式来处理
	bool isSend = (Qt::Key_Return==event->key() || Qt::Key_Enter==event->key())
		&& (event->modifiers() & Qt::ControlModifier);

	if(isSend)
		emit OnSendMessage();
	else
		QTextEdit::keyPressEvent(event);
}