#pragma once

#include "../common/Qt.h"
#include "../common/stl.h"
#include "VerificationCodeImage.h"
#include "LoginEdit.h"
#include "LoginButton.h"
#include "LoginCheckBox.h"
#include "LoginLabel.h"
#include "MobileActivity.h"

class LoginActivity : public MobileActivity
{
	Q_OBJECT
public:
	LoginActivity(MainWindow* mainWindow, QWidget *parent = 0);
	virtual ~LoginActivity();

	virtual void OnBackKey();
	virtual void OnPause();

Q_SIGNALS:
	void OnLoginSuccess();

private Q_SLOTS:
	void LoginTriggered();
	void OfflineTriggered();
	void LoginFinishedTriggered(bool value, QString reason);
	void RegisterTriggered();
	void BrowserTriggered();

private:
	virtual void paintEvent(QPaintEvent *e);

private:
	LoginEdit *_user;
	LoginEdit *_password;
	LoginEdit *_verify;

	LoginCheckBox* _rememberPassword;
};

