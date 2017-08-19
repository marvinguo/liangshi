#include "BanTitleWidget.h"
#include "BanBrowserDialog.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BanTitleWidget::BanTitleWidget(QWidget *parent):
QWidget(parent)
{
	_insert = new TitleButton(tr("插入板书"));
	_insert->setToolTip(tr("插入别的板书"));
	connect(_insert, &TitleButton::clicked, this, &BanTitleWidget::OnInsertTriggered);


	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->addWidget(_insert);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	setFixedWidth(35*2);
	setFixedHeight(22);
	setLayout(mainLayout);

	BanBrowserDialog* dialog = BanBrowserDialog::Instance();
	connect(dialog, &BanBrowserDialog::OnScFileSelected, this, &BanTitleWidget::OnInsertScFile);
}

BanTitleWidget::~BanTitleWidget()
{
}

void BanTitleWidget::OnInsertTriggered()
{
	BanBrowserDialog* dialog = BanBrowserDialog::Instance();
	dialog->ShowModal();
}

