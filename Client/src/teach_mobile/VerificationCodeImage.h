#pragma once

#include "../common/Qt.h"

class MainWindow;

class VerificationCodeImage : public QLabel
{
	Q_OBJECT
public:
	VerificationCodeImage(MainWindow* mainWindow, QWidget *parent = 0);
	virtual ~VerificationCodeImage();

private:
	virtual void mousePressEvent(QMouseEvent *e);

Q_SIGNALS:
	void DoneAPK();

private Q_SLOTS:
	void OnCheckVersionTriggered(int version);
	void OnGetUpdateUrlTriggered(QString url);
	void DownloadAPKFileTriggered(QString localFile);
	void DownloadImageFileTriggered(QString localFile);

private:
	void RefreshImage();

private:
	MainWindow* _mainWindow;
};

