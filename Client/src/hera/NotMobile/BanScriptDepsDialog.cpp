#include "BanScriptDepsDialog.h"
#include "../common/FileUtil.h"
#include "ChineseMessageBox.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BanScriptDepsDialog::BanScriptDepsDialog():
QtSingleton<BanScriptDepsDialog>(this)
{
	setWindowIcon(QIcon(":/App/collect"));
	setWindowTitle(QObject::tr("程序板书依赖的文件"));
	Qt::WindowFlags flags = windowFlags();
	flags = flags & (~Qt::WindowContextHelpButtonHint);
	setWindowFlags(flags);
	setModal(true);

	_fileList = new QListWidget();
	connect(_fileList, &QListWidget::itemSelectionChanged, this, &BanScriptDepsDialog::OnListItemChangedTriggered);
	connect(_fileList, &QListWidget::itemDoubleClicked, this, &BanScriptDepsDialog::OnListItemDoubleClickedTriggered);

	QHBoxLayout* titlebar = new QHBoxLayout();
	_addButton = new TitleButton(tr("添加"));
	_deleteButton = new TitleButton(tr("删除"));
	_saveAsButton = new TitleButton(tr("另存"));

	connect(_addButton, &TitleButton::clicked, this, &BanScriptDepsDialog::OnAddTriggered);
	connect(_deleteButton, &TitleButton::clicked, this, &BanScriptDepsDialog::OnDeleteTriggered);
	connect(_saveAsButton, &TitleButton::clicked, this, &BanScriptDepsDialog::OnSaveAsTriggered);

	titlebar->addWidget(_addButton);
	titlebar->addWidget(_deleteButton);
	titlebar->addWidget(_saveAsButton);

	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addLayout(titlebar);
	mainLayout->addWidget(_fileList);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(mainLayout);

	setMinimumWidth(500);
	setMinimumHeight(200);
}

BanScriptDepsDialog::~BanScriptDepsDialog()
{
}

void BanScriptDepsDialog::ShowModal(BanScriptDeps* deps, BanScriptWidget* widget)
{
	_widget = widget;
	_deps = deps;
	_fileList->clear();
	_deleteButton->setEnabled(false);
	_saveAsButton->setEnabled(false);
	vector<QString> filenames = deps->GetFileNames();

	for(vector<QString>::iterator i = filenames.begin(); i != filenames.end(); ++i)
	{
		QListWidgetItem* item = new QListWidgetItem(*i);
		item->setData(Qt::UserRole, QVariant(*i));
		_fileList->addItem(item);
	}
	show();
	exec();
}

void BanScriptDepsDialog::OnListItemChangedTriggered()
{
	_deleteButton->setEnabled(true);
	_saveAsButton->setEnabled(true);
}

void BanScriptDepsDialog::OnListItemDoubleClickedTriggered(QListWidgetItem *item)
{
	QString filename = (item->data(Qt::UserRole)).toString();
	_deps->OpenFile(filename);
}

void BanScriptDepsDialog::OnAddTriggered()
{
	QString path = QFileDialog::getOpenFileName(NULL, QObject::tr("添加文件到程序"), ".", QObject::tr("所有支持文件(*.html *.htm *.mml *.txt *.ban *.js *.jpg *.jpeg *.png *.gif);;板书文件(*.ban);;程序文件(*.js);;图片文件(*.jpg *.jpeg *.png *.gif);;Html文件(*.html *.htm);;MathML文件(*.mml);;文本文件(*.txt)"));
	if(!path.isEmpty())
	{
		QFileInfo fileInfo(path);
		QString filename = fileInfo.fileName();

		//check filename
		QString fullpath;
		if(_deps->HasDepsFile(filename, fullpath))
		{
			int rb = ChineseMessageBox::Show(QString(tr("%1文件已经存在，是否替换?")).arg(filename), tr("提醒"), ChineseMessageBox::YesNo, ChineseMessageBox::Question);
			if(rb == QMessageBox::No)
			{
				return;
			}
		}

		//找到，删除
		QList<QListWidgetItem*> items = _fileList->findItems(filename, Qt::MatchExactly);
		foreach (QListWidgetItem* item, items)
		{
			_fileList->removeItemWidget(item);
			delete item;
		}

		QListWidgetItem* item = new QListWidgetItem(filename);
		item->setData(Qt::UserRole, QVariant(filename));
		_fileList->addItem(item);
		_deps->AddFile(path);
		_widget->SetModifiedTriggered();
	}
}

void BanScriptDepsDialog::OnDeleteTriggered()
{
	foreach (const QListWidgetItem* item, _fileList->selectedItems())
	{
		QString filename = (item->data(Qt::UserRole)).toString();
		_deps->Remove(filename);
		_widget->SetModifiedTriggered();
	}

	qDeleteAll(_fileList->selectedItems());
}

void BanScriptDepsDialog::OnSaveAsTriggered()
{
	foreach (const QListWidgetItem* item, _fileList->selectedItems())
	{
		QString filename = (item->data(Qt::UserRole)).toString();
		QFileInfo fileInfo(filename);

		QString path = QFileDialog::getSaveFileName(this, tr("另存为文件"), fileInfo.fileName(), "");
		if(!path.isEmpty())
		{
			_deps->CopyFileTo(filename, path);
		}
	}
}