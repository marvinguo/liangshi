#include "LoginActivity.h"
#include "../common/YunHttp.h"
#include "LocalSave.h"
#include "LoginTitleLabel.h"
#include "LoginBottomLabel.h"
#include "../hera/ChineseMessageBox.h"
#include "../hera/NotifyWindow.h"
#include "MainWindow.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

LoginActivity::LoginActivity(MainWindow* mainWindow, QWidget *parent):
MobileActivity(mainWindow, parent),
_user(NULL),
_password(NULL)
{
	setWindowTitle("登录");
	QVBoxLayout *layout = new QVBoxLayout();

	QHBoxLayout* titleH = new QHBoxLayout();
	LoginTitleLabel* title = new LoginTitleLabel(tr("良师益友"));
	titleH->addWidget(title);
	layout->addLayout(titleH);

	layout->addStretch(30);

	QHBoxLayout* first = new QHBoxLayout();
	_user = new LoginEdit();
	first->addWidget(new LoginLabel(tr("手机:")));
	first->addWidget(_user);
	layout->addLayout(first);

	QHBoxLayout* second = new QHBoxLayout();
	_password = new LoginEdit();
	//setPlaceholderText
	_password->setEchoMode(QLineEdit::Password);
	second->addWidget(new LoginLabel(tr("密码:")));
	second->addWidget(_password);
	_rememberPassword = new LoginCheckBox(tr("保存密码"));
	second->addWidget(_rememberPassword);
	layout->addLayout(second);

	//加入验证码
	QHBoxLayout* third = new QHBoxLayout();
	_verify = new LoginEdit();
	_verify->setValidator(new QIntValidator(0, 9999, this));
	third->addWidget(new LoginLabel(tr("验证码:")));
	third->addWidget(new VerificationCodeImage(mainWindow));
	third->addWidget(_verify);
	layout->addLayout(third);

	QHBoxLayout* four = new QHBoxLayout();
	LoginButton *login = new LoginButton(tr("登录"));
	login->setDefault(true);
	LoginButton *offline = new LoginButton(tr("离线学习"));
	four->addWidget(login);
	four->addWidget(offline);
	layout->addLayout(four);
	connect(login, &LoginButton::clicked, this, &LoginActivity::LoginTriggered);
	connect(offline, &LoginButton::clicked, this, &LoginActivity::OfflineTriggered);

	QHBoxLayout* five = new QHBoxLayout();
	LoginButton *reg = new LoginButton(tr("注册用户"));
	LoginButton *browser = new LoginButton(tr("浏览网站"));
	connect(reg, &LoginButton::clicked, this, &LoginActivity::RegisterTriggered);
	connect(browser, &LoginButton::clicked, this, &LoginActivity::BrowserTriggered);
	five->addWidget(reg);
	five->addWidget(browser);
	layout->addLayout(five);

	QHBoxLayout* six = new QHBoxLayout();
	LoginBottomLabel *info = new LoginBottomLabel(tr("良师益友学习网版权所有, 版本1.1.34. \n联系电话: 18547298472, 联系人: 戴老师"));
	six->addWidget(info, 0, Qt::AlignHCenter);
	layout->addLayout(six);

	layout->addStretch(1);
	setLayout(layout);

	LocalSave save;
	_user->setText(save._username);
	_password->setText(save._password);
	_rememberPassword->setChecked(save._rememmberPassword);
}

LoginActivity::~LoginActivity()
{
}

void LoginActivity::OnBackKey()
{
	int rb = ChineseMessageBox::Show(tr("是否退出?"), tr("注意"), ChineseMessageBox::YesNo, ChineseMessageBox::Question);
	switch(rb)
	{
	case QMessageBox::Yes:
		_mainWindow->close();
		break;

	case QMessageBox::No:
		break;
	}
}

void LoginActivity::OnPause()
{
}

void LoginActivity::paintEvent(QPaintEvent *e)
{
	QPixmap pixmap(":/App/background");

	QRect r = rect();
	QSize size(r.width(), r.height() - 100);
	QPixmap scaledPix = pixmap.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	QPainter painter(this);
	painter.drawPixmap(0, 0, scaledPix.width(), scaledPix.height(), scaledPix);
	QWidget::paintEvent(e);
}

void LoginActivity::LoginTriggered()
{
	QString user = _user->text();
	QString password = _password->text();
	QString verify = _verify->text();

	LocalSave save;
	YunHttp* http = new YunHttp(save._cookie);

	QString deviceID= Util::GetAndroidDeviceID();
	connect(http, &YunHttp::MobileLoginFinished, this, &LoginActivity::LoginFinishedTriggered);
	http->MobileLogin(user, password, deviceID, verify);
}

void LoginActivity::OfflineTriggered()
{
	_mainWindow->OnGotoOffline();
}

void LoginActivity::RegisterTriggered()
{
	Util::OpenAndroidWebsite(tr("http://www.liangshiyy.com/Register"));
}

void LoginActivity::BrowserTriggered()
{
	Util::OpenAndroidWebsite(tr("http://www.liangshiyy.com"));
}

void LoginActivity::LoginFinishedTriggered(bool value, QString reason)
{
	YunHttp* http = qobject_cast<YunHttp *>(sender());
	if(http != NULL)
		delete http;

	if(value)
	{
		QString user = _user->text();
		QString password = _password->text();
		QString verify = _verify->text();

		LocalSave save;
		save._username = user;
		if(_rememberPassword->isChecked())
			save._password = password;
		else
			save._password = "";
		save._rememmberPassword = _rememberPassword->isChecked();

		Q_EMIT OnLoginSuccess();
	}
	else
	{
		ChineseMessageBox::Show(reason, tr("错误"), ChineseMessageBox::OK, ChineseMessageBox::Error);
		//QMessageBox::information(this, tr("错误"), reason, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
}

