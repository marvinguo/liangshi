#include "LokiLoginDialog.h"
#include "LokiClient.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

LokiLoginDialog::LokiLoginDialog(QWidget *parent):
QDialog(parent),
_result(Quit)
{
	_ui.setupUi(this);
	Qt::WindowFlags flags = windowFlags();
	flags = flags & (~Qt::WindowContextHelpButtonHint);
	setWindowFlags(flags);

	QPalette palette;
	QPixmap pixmap(":/Resources/Images/loginBackground.jpg");
	palette.setBrush(QPalette::Window, QBrush(pixmap));
	setPalette(palette);

	setWindowTitle(tr("登录"));
	setWindowIcon(QIcon(":/Resources/AppIcons/teach.png"));

	_ui._password->setEchoMode(QLineEdit::Password);

	connect(_ui._register, SIGNAL(clicked()), this, SLOT(OnRegisterTriggered()));
	connect(_ui._login, SIGNAL(clicked()), this, SLOT(OnLoginTriggered()));

	LokiClient& lokiClient = LokiClient::GetInstance();
	connect(&lokiClient, SIGNAL(OnLogin(bool, QString&)), this, SLOT(OnLoginResultTriggered(bool, QString&)));
}

LokiLoginDialog::~LokiLoginDialog()
{
}

bool LokiLoginDialog::Login()
{
	int k = exec();
	//http://stackoverflow.com/questions/12470806/qdialog-exec-and-getting-result-value
	return false;
}

void LokiLoginDialog::OnRegisterTriggered()
{
	QDesktopServices::openUrl(QUrl(REGISTER_URL));
}

void LokiLoginDialog::OnLoginTriggered()
{
	LokiClient& lokiClient = LokiClient::GetInstance();
	lokiClient.Init(LOKI_SERVER, LOKI_LISTEN_PORT);

	QString user = _ui._account->text();
	QString password = _ui._password->text();
	QString md5;
	QByteArray byte = QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Md5);
	md5.append(byte.toHex());
	lokiClient.Login(user, md5);
}

void LokiLoginDialog::OnLoginResultTriggered(bool result, QString& message)
{
	if(result)
	{
		_result = LoginSuccess;
		close();
	}
	else
	{
		QMessageBox::warning(this, tr("错误"), tr("用户名或密码错误"));
	}
}