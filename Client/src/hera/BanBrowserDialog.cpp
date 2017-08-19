#include "BanBrowserDialog.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BanBrowserDialog::BanBrowserDialog():
QtSingleton<BanBrowserDialog>(this),
_lastFolder("")
{
	setWindowIcon(QIcon(":/App/collect"));
	setWindowTitle(QObject::tr("插入板书"));
	Qt::WindowFlags flags = windowFlags();
	flags = flags & (~Qt::WindowContextHelpButtonHint);
	setWindowFlags(flags);
	setModal(true);

	QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
	splitter->setStretchFactor(1, 10);

	BanCategoryList* categoryList = new BanCategoryList(splitter);
	_banList = new BanList(splitter);

	connect(categoryList, &BanCategoryList::OnFolderSelected, _banList, &BanList::OnFolderTriggered);
	connect(categoryList, &BanCategoryList::OnFolderSelected, this, &BanBrowserDialog::OnFolderTriggered);

	connect(_banList, &BanList::OnScFileSelect, this, &BanBrowserDialog::OnScFileSelected);

	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->addWidget(splitter);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(mainLayout);

	setMinimumWidth(800);
	setMinimumHeight(700);
}

BanBrowserDialog::~BanBrowserDialog()
{
}

void BanBrowserDialog::OnFolderTriggered(QString path)
{
	_lastFolder = path;
}

void BanBrowserDialog::ShowModal()
{
	show();
	//这里需要update一下
	if(_lastFolder != "")
		_banList->OnFolderTriggered(_lastFolder);
	exec();
}
