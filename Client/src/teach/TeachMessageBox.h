#pragma once

#include <QtWidgets>

class TeachMessageBox: public QObject
{
	Q_OBJECT
public:
	TeachMessageBox(QWidget *parent = 0);
	~TeachMessageBox();

	void ShowWaitingForConnection();
	void ShowSyncing();
	void HideMsgBox();

Q_SIGNALS:
	void OnCancelWaitingForConnection();
	void OnCancelSyncing();

private Q_SLOTS:
	void msgBoxClosed(QAbstractButton* button);

private:
	int _type;
	QMessageBox* _msgBox;
	QWidget* _parentWidget;
};
