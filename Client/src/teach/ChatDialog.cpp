#include "ChatDialog.h"
#include "../common/LokiClient.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ChatDialog::ChatDialog(QWidget *parent/* =0 */):
QDialog(parent)
{
	_ui.setupUi(this);

	setWindowTitle(tr("沟通"));
	setWindowIcon(QIcon(":/Resources/AppIcons/teach.png"));

	QPalette palette;
	QPixmap pixmap(":/Resources/Images/teachBackground.jpg");
	palette.setBrush(QPalette::Window, QBrush(pixmap));
	setPalette(palette);

	OpenChatFile();

	connect(_ui._send, SIGNAL(clicked()), this, SLOT(OnSendTriggered()));
	connect(_ui._material, SIGNAL(clicked()), this, SLOT(OnMaterialTriggered()));
	connect(_ui._color, SIGNAL(clicked()), this, SLOT(OnColorTriggered()));
	connect(_ui._font, SIGNAL(clicked()), this, SLOT(OnFontTriggered()));
	connect(_ui._close, SIGNAL(clicked()), this, SLOT(close()));
	connect(_ui._inputMessage, SIGNAL(OnSendMessage()), this, SLOT(OnSendTriggered()));
	connect(_ui._inputMessage, SIGNAL(textChanged()), this, SLOT(InputTextChangeTriggered()));
}

ChatDialog::~ChatDialog()
{

}

void ChatDialog::InputTextChangeTriggered()
{
	if(_ui._inputMessage->toPlainText().isEmpty())
	{
		_ui._inputMessage->setTextColor(_chatFile.color);
		_ui._inputMessage->setCurrentFont(_chatFile.font);
	}
}

void ChatDialog::SaveChatFile()
{
	LokiClient& lokiClient = LokiClient::GetInstance();
	QString login_name = lokiClient.login_name;
	QString filename = QString("./save/%1/chat.cf").arg(login_name);

	filename = QDir::toNativeSeparators(filename);
	QFile file(filename);
	if( !file.open(QIODevice::WriteOnly) )
		return;
	QDataStream out(&file);
	out.setVersion(QDataStream::Qt_4_0);
	out << _chatFile;
	file.close();
}

void ChatDialog::OpenChatFile()
{
	LokiClient& lokiClient = LokiClient::GetInstance();
	QString login_name = lokiClient.login_name;
	QString filename = QString("./save/%1/chat.cf").arg(login_name);


	filename = QDir::toNativeSeparators(filename);
	QFile file(filename);

	if( !file.open(QIODevice::ReadOnly) )
		return;

	QDataStream in(&file);
	in.setVersion(QDataStream::Qt_4_0);
	in >> _chatFile;
	_ui._inputMessage->setCurrentFont(_chatFile.font);
	_ui._inputMessage->setTextColor(_chatFile.color);
	file.close();
}

void ChatDialog::OnColorTriggered()
{
	QColorDialog *colorDialog = new QColorDialog(this);
	colorDialog->setCurrentColor(_ui._inputMessage->textColor());

	if(QDialog::Accepted == colorDialog->exec())
	{
		_chatFile.color = colorDialog->currentColor();
		_ui._inputMessage->setTextColor(_chatFile.color);
		SaveChatFile();
	}
}

void ChatDialog::OnFontTriggered()
{
	QFontDialog *fontDialog = new QFontDialog(this);
	fontDialog->setCurrentFont(_ui._inputMessage->font());
	fontDialog->setModal(true);
	if(QDialog::Accepted == fontDialog->exec())
	{
		_chatFile.font = fontDialog->currentFont();
		_ui._inputMessage->setCurrentFont(_chatFile.font);
		SaveChatFile();
	}
}

void ChatDialog::OnSendTriggered()
{
	QString message = _ui._inputMessage->toHtml();
	//用html，这样就可以有颜色等，用Qt就是厉害
	_ui._inputMessage->clear();
//	QString dateTime = getCurrentDateTime();
//	QString temp = QString("<font size=\"4\" color=green>%1    %2: </font>%3").arg(myName).arg(dateTime).arg(sendString);
	_ui._allMessage->append(message);

	LokiClient& lokiClient = LokiClient::GetInstance();
	lokiClient.Chat(lokiClient.user_id, _info.user_id, message);
}

void ChatDialog::InsertReceivedMessage(QString& message)
{
	_ui._allMessage->append(message);
}

/*
9.17，我想是这样，先可以打开素材，或是从课件里找到一个，调整时间，完成素材，可以录音，f2什么的加入声音
先做离线，就相当于是问老师

还可以做实时

看具体怎么个情况了
*/

void ChatDialog::OnMaterialTriggered()
{

}