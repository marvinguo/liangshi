#include "ChineseMessageBox.h"
#include "../common/FileUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ChineseMessageBox::ChineseMessageBox()
{
}

ChineseMessageBox::~ChineseMessageBox()
{
}

QString ChineseMessageBox::GetButtonText(QMessageBox::StandardButton standar)
{
	qDebug() << "ChineseMessageBox::GetButtonText:" << standar;
	switch (standar)
	{
	case QMessageBox::Ok:
		return QString(QObject::tr("确定"));
	case QMessageBox::Cancel:
		return QString(QObject::tr("取消"));
	case QMessageBox::Abort:
		return QString(QObject::tr("终止"));
	case QMessageBox::Retry:
		return QString(QObject::tr("重试"));
	case QMessageBox::Ignore:
		return QString(QObject::tr("忽略"));
	case QMessageBox::Yes:
		return QString(QObject::tr("是"));
	case QMessageBox::No:
		return QString(QObject::tr("否"));
	default:
		return QString(QObject::tr("确定"));
	}
}

QMap<QString,QMessageBox::StandardButton> ChineseMessageBox::GetButton(MessageBoxButtons type)
{
	QMap<QString,QMessageBox::StandardButton> map;
	switch (type)
	{
	case MessageBoxButtons(0):
	default:
		map.insert(GetButtonText(QMessageBox::Ok), QMessageBox::Ok);
		break;
	case MessageBoxButtons(1):
		map.insert(GetButtonText(QMessageBox::Ok), QMessageBox::Ok);
		map.insert(GetButtonText(QMessageBox::Cancel), QMessageBox::Cancel);
		break;
	case MessageBoxButtons(2):
		map.insert(GetButtonText(QMessageBox::Abort), QMessageBox::Abort);
		map.insert(GetButtonText(QMessageBox::Retry), QMessageBox::Retry);
		map.insert(GetButtonText(QMessageBox::Ignore), QMessageBox::Ignore);
		break;
	case MessageBoxButtons(3):
		map.insert(GetButtonText(QMessageBox::Yes), QMessageBox::Yes);
		map.insert(GetButtonText(QMessageBox::No), QMessageBox::No);
		map.insert(GetButtonText(QMessageBox::Cancel), QMessageBox::Cancel);
		break;
	case MessageBoxButtons(4):
		map.insert(GetButtonText(QMessageBox::Yes), QMessageBox::Yes);
		map.insert(GetButtonText(QMessageBox::No), QMessageBox::No);
		break;
	case MessageBoxButtons(5):
		map.insert(GetButtonText(QMessageBox::Retry), QMessageBox::Retry);
		map.insert(GetButtonText(QMessageBox::Cancel), QMessageBox::Cancel);
		break;
	}
	return map;
}

int ChineseMessageBox::Show(const QString& content, const QString& caption, MessageBoxButtons button, MessageBoxIcon icon)
{
	QMessageBox::Icon con;
	switch(icon)
	{
	case MessageBoxIcon(0):
		con = QMessageBox::NoIcon;
		break;
	case MessageBoxIcon(1):
	case MessageBoxIcon(5):
	case MessageBoxIcon(6):
		con = QMessageBox::Critical;
		break;
	case MessageBoxIcon(2):
		con = QMessageBox::Question;
		break;
	case MessageBoxIcon(3):
	case MessageBoxIcon(7):
		con = QMessageBox::Warning;
		break;
	default:
		con = QMessageBox::Information;
		break;
	}
	QMap<QString,QMessageBox::StandardButton> ms = GetButton(button);
	QMessageBox box(con, caption, content);
	box.setWindowIcon(QIcon(":/App/collect"));
	int size = ms.size ();
	switch(size)
	{
	case 1:
		box.setStandardButtons (ms.first ());
		box.setButtonText (ms.values().at (0),ms.keys ().at (0));
		break;
	case 2:
		box.setStandardButtons (ms.first () | ms.last ());
		box.setButtonText (ms.values().at (0),ms.keys ().at (0));
		box.setButtonText (ms.values().at (1),ms.keys ().at (1));
		break;
	case 3:
		box.setStandardButtons (ms.values ().at (0)|ms.values ().at (1)|ms.values ().at (2));
		box.setButtonText (ms.values().at (0),ms.keys ().at (0));
		box.setButtonText (ms.values().at (1),ms.keys ().at (1));
		box.setButtonText (ms.values().at (2),ms.keys ().at (2));
		break;
	}
	return box.exec();
}

int ChineseMessageBox::Show(const QString& content, const QString& caption, MessageBoxButtons button)
{
	return Show(content, caption, button, MessageBoxIcon(0));
}

int ChineseMessageBox::Show(const QString& content, const QString& caption)
{
	return Show(content, caption, MessageBoxButtons(0));
}

int ChineseMessageBox::Show(const QString& content)
{
	return Show(content, "");
}