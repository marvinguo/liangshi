#include "ElementResizableTitlebar.h"
#include "ImageElement.h"
#include "BanBaseWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ElementResizableTitlebar::ElementResizableTitlebar(QWidget *parent):
QWidget(parent)
{
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	//加入到顶层，到底层
	_delete = new TitleButton(tr("删除"));
	_edit = new TitleButton(tr("编辑"));
	_title = new TitleButton(tr("标签"));
	_topMost = new TitleButton(tr("到最上"));
	_bottomMost = new TitleButton(tr("到最下"));

	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->addWidget(_delete);
	mainLayout->addWidget(_edit);
	mainLayout->addWidget(_title);
	mainLayout->addWidget(_topMost);
	mainLayout->addWidget(_bottomMost);
	mainLayout->addStretch();
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(mainLayout);

	_delete->hide();
	_edit->hide();
	_title->hide();
	_topMost->hide();
	_bottomMost->hide();
}

ElementResizableTitlebar::~ElementResizableTitlebar()
{
}
