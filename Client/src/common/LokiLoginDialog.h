#pragma once

#include "Qt.h"
#include "stl.h"
#include "config.h"

#include "ui_LokiLoginDialog.h"

class LokiLoginDialog : public QDialog
{
	Q_OBJECT
public:
	enum Result
	{
		LoginSuccess,
		LoginFail,
		Quit,
	};

public:
	LokiLoginDialog(QWidget *parent = 0);
	virtual ~LokiLoginDialog();

	bool Login();
	Result GetResult(){return _result;}

private Q_SLOTS:
	void OnRegisterTriggered();
	void OnLoginTriggered();
	void OnLoginResultTriggered(bool result, QString& message);

private:
	Ui::LokiLoginDialog _ui;
	Result _result;
};

