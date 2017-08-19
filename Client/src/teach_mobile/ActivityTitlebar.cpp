#include "ActivityTitlebar.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ActivityTitlebar::ActivityTitlebar(TitlebarType type, QString title, QWidget* parent):
QWidget(NULL)
{
	QHBoxLayout* mainLayout = new QHBoxLayout();
//	mainLayout->setSizeConstraint(QLayout::SetNoConstraint);

	ActivityTitleButton* back = new ActivityTitleButton(tr("<后退"));
	mainLayout->addWidget(back);
	connect(back, &ActivityTitleButton::clicked, this, &ActivityTitlebar::OnBack);

	if(type == TitlebarType_Normal)
	{
		_titleLabel = new ActivityTitleLabel(title);
		mainLayout->addWidget(_titleLabel);
	}

	if(type == TitlebarType_Player)
	{
		_play = new PlayTitlebar();
		mainLayout->addWidget(_play, 10);
	}

	if(type == TitlebarType_Ke)
	{
		_ke = new KeTitlebar();
		mainLayout->addWidget(_ke);
	}

	if(type == TitlebarType_Live)
	{
		_live = new LiveTitlebar();
		mainLayout->addWidget(_live);
	}

//	mainLayout->addStretch();
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(mainLayout);
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	//setFixedHeight(30);
}

void ActivityTitlebar::SetTitleText(QString text)
{
	QFontMetrics metrics(_titleLabel->font());
	int width = 400;//_titleLabel->width();
	QString elidedText = metrics.elidedText(text, Qt::ElideRight, width);
	_titleLabel->setText(elidedText);
}

