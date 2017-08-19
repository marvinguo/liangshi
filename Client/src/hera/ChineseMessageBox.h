#pragma once

#include "../common/Qt.h"
#include "../common/library.h"

class LIANGSHIEXPORT ChineseMessageBox
{
public:
	enum MessageBoxButtons
	{
		OK = 0,                         //消息框包含“确定”按钮
		OKCancel = 1,                   //消息框包含“确定”和“取消”按钮
		AbortRetryIgnore = 2,           //消息框包含“中止”、“重试”和“忽略”按钮
		YesNoCancel = 3,                //消息框包含“是”、“否”和“取消”按钮
		YesNo = 4,                      //消息框包含“是”和“否”按钮
		RetryCancel = 5                 //消息框包含“重试”和“取消”按钮
	};

	enum MessageBoxIcon
	{
		None = 0,                       //消息框未包含符号
		Hand = 1,                       //该消息框包含一个符号，该符号是由一个红色背景的圆圈及其中的白色 X 组成的
		Question = 2,                   //该消息框包含一个符号，该符号是由一个圆圈和其中的一个问号组成的
		Exclamation = 3,                //该消息框包含一个符号，该符号是由一个黄色背景的三角形及其中的一个感叹号组成的
		Asterisk = 4,                   //该消息框包含一个符号，该符号是由一个圆圈及其中的小写字母 i 组成的
		Stop = 5,                       //该消息框包含一个符号，该符号是由一个红色背景的圆圈及其中的白色 X 组成的
		Error = 6,                      //该消息框包含一个符号，该符号是由一个红色背景的圆圈及其中的白色 X 组成的
		Warning = 7,                    //该消息框包含一个符号，该符号是由一个黄色背景的三角形及其中的一个感叹号组成的
		Information = 8                 //该消息框包含一个符号，该符号是由一个圆圈及其中的小写字母 i 组成的
	};
public:
	ChineseMessageBox();
	~ChineseMessageBox();

public:
	static int Show(const QString & content);
	static int Show(const QString & content, const QString & caption);
	static int Show(const QString & content, const QString & caption, MessageBoxButtons button);
	static int Show(const QString & content, const QString & caption, MessageBoxButtons button, MessageBoxIcon icon);

private:
	static QMap<QString,QMessageBox::StandardButton> GetButton(MessageBoxButtons type);
	static QString GetButtonText(QMessageBox::StandardButton standar);
};

