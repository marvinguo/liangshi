#pragma once
#include "../common/Qt.h"
#include "../common/library.h"
#include "../common/QtSingleton.h"

class LIANGSHIEXPORT NotifyWindow : public QWidget, public QtSingleton<NotifyWindow>
{
	Q_OBJECT
public:
	NotifyWindow(QWidget* parent = NULL);
	~NotifyWindow();
	void ShowNotify(QString text, int position = -1);

private Q_SLOTS:
	void OnHideTriggered();

private:
	int _delay;
	QLabel* _text;
	QTimer* _timer;
};

