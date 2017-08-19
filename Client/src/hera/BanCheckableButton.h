#pragma once

#include "../common/Qt.h"
#include "../common/library.h"

class LIANGSHIEXPORT ScCheckableButton : public QToolButton
{
	Q_OBJECT
public:
	ScCheckableButton(QString tooltip, QWidget *parent = 0);
	~ScCheckableButton();

	void UnCheck();

Q_SIGNALS:
	void OnChecked();
	void OnUnChecked();

private Q_SLOTS:
	void OnCheckedTriggered(bool checked);
};

