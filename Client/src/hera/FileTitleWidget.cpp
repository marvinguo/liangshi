#include "FileTitleWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

FileTitleWidget::FileTitleWidget(FileTitleType type, QWidget *parent):
QWidget(parent)
{
	_open = new OneImageButton(":/Icons/open", tr("打开文件"));
	_save = new OneImageButton(":/Icons/save", tr("保存文件"));
	_saveAs = new OneImageButton(":/Icons/saveas", tr("另存为文件"));

	//connect(_open, &QToolButton::clicked, this, &SysTitleWidget::OnMinTriggered);
	//connect(_save, &QToolButton::clicked, this, &SysTitleWidget::OnMaxTriggered);

	QHBoxLayout* mainLayout = new QHBoxLayout();
	if(type == FileTitleType_WithOpenWithoutSave)
	{
		mainLayout->addWidget(_open);
		setFixedWidth(35);
	}
	else if(type == FileTitleType_WithSaveWithoutOpen)
	{
		mainLayout->addWidget(_save);
		mainLayout->addWidget(_saveAs);
		setFixedWidth(35*2);
	}
	else
	{
		mainLayout->addWidget(_save);
		mainLayout->addWidget(_saveAs);
		setFixedWidth(35*2);
	}
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	setFixedHeight(22);
	setLayout(mainLayout);
}

FileTitleWidget::~FileTitleWidget()
{
	if(_open != NULL)
	{
		_open->deleteLater();
		_open = NULL;
	}
	if(_save != NULL)
	{
		_save->deleteLater();
		_save = NULL;
	}
	if(_saveAs != NULL)
	{
		_saveAs->deleteLater();
		_saveAs = NULL;
	}
}

