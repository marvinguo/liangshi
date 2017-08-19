#include "ScriptTitleWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ScriptTitleWidget::ScriptTitleWidget(QWidget *parent):
QWidget(parent)
{
	_run = new TitleButton(tr("运行"));
	_debug = new TitleButton(tr("调试"));
	_editor = new TitleButton(tr("编辑"));
	_deps = new TitleButton(tr("依赖"));

	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->addWidget(_run);
	mainLayout->addWidget(_debug);
	mainLayout->addWidget(_editor);
	mainLayout->addWidget(_deps);
	setFixedWidth(35*4);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	setFixedHeight(22);
	setLayout(mainLayout);
}

ScriptTitleWidget::~ScriptTitleWidget()
{

}
