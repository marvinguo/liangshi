#include "BanCheckableButton.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ScCheckableButton::ScCheckableButton(QString tooltip, QWidget *parent):
QToolButton(parent)
{
	setCheckable(true);
	//setFixedSize(22, 22);
	setFixedHeight(22);
	//setAutoRaise(true);
	setToolTip(tooltip);

	connect(this, &ScCheckableButton::toggled, this, &ScCheckableButton::OnCheckedTriggered);
}

ScCheckableButton::~ScCheckableButton()
{
}

void ScCheckableButton::UnCheck()
{
	disconnect(this, &ScCheckableButton::toggled, this, &ScCheckableButton::OnCheckedTriggered);
	setChecked(false);
	connect(this, &ScCheckableButton::toggled, this, &ScCheckableButton::OnCheckedTriggered);
}

void ScCheckableButton::OnCheckedTriggered(bool checked)
{
	if(checked)
	{
		Q_EMIT OnChecked();
	}
	else
	{	
		disconnect(this, &ScCheckableButton::toggled, this, &ScCheckableButton::OnCheckedTriggered);
		setChecked(true);
		connect(this, &ScCheckableButton::toggled, this, &ScCheckableButton::OnCheckedTriggered);
		//Q_EMIT OnUnChecked();
	}
}


