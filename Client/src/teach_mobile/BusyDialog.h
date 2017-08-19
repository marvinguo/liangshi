#pragma once

#include "../common/Qt.h"
#include "../common/library.h"
#include "../common/YunHttp.h"
#include "busyindicator.h"

class BusyDialog : public QDialog
{
	Q_OBJECT
public:
	BusyDialog(QWidget* parent = 0);
	void ShowModal();

public Q_SLOTS:
	void Done();

private:
	BusyIndicator* _busy;
};

