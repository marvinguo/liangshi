#include "MainWindow.h"
#include "../common/config.h"
#include "../hera/ChineseMessageBox.h"
#include "../hera/KeFile.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow():
QMainWindow(NULL),
_keWidget(NULL),
_keTime(new KeTime(this))
{
	setWindowIcon(QIcon(":/App/magic"));

	CreateActions();
	CreateToolBar();
	CreateDock();
}

MainWindow::~MainWindow()
{
	if(_keWidget != NULL)
	{
		delete _keWidget;
		_keWidget = NULL;
	}
}

void MainWindow::CreateActions()
{
	_openAction = new QAction(tr("打开"), this);
	_openAction->setShortcuts(QKeySequence::Open);
	//_openAction->setStatusTip(tr("Create a new file"));
	connect(_openAction, &QAction::triggered, this, &MainWindow::OnOpenTriggered);

	_saveAction = new QAction(tr("保存"), this);
	connect(_saveAction, &QAction::triggered, this, &MainWindow::OnSaveTriggered);
}

void MainWindow::CreateDock()
{
	QDockWidget* dock = new QDockWidget(tr("Actions"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	_actionTable = new ActionTable(dock);
	dock->setWidget(_actionTable);
	addDockWidget(Qt::RightDockWidgetArea, dock);
	_toolBar->addAction(dock->toggleViewAction());
	dock = new QDockWidget(tr("Bans"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	_banTable = new BanTable(dock);
	dock->setWidget(_banTable);
	addDockWidget(Qt::RightDockWidgetArea, dock);
	_toolBar->addAction(dock->toggleViewAction());
	dock = new QDockWidget(tr("Elements"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	_elementTable = new ElementTable(dock);
	dock->setWidget(_elementTable);
	addDockWidget(Qt::RightDockWidgetArea, dock);
	_toolBar->addAction(dock->toggleViewAction());
}

void MainWindow::CreateToolBar()
{
	_toolBar = addToolBar(tr("标准"));
	_toolBar->addAction(_openAction);
	_toolBar->addAction(_saveAction);
}

void MainWindow::OnOpenTriggered()
{
	QString path = QFileDialog::getOpenFileName(this, QObject::tr("打开课件文件"), ".", QObject::tr("课件文件(*.ke)"));
	if(!path.isEmpty())
	{
		if(_keWidget != NULL)
			delete _keWidget;
		_keWidget = KeFile::LoadKeAsKeWidget(path);

		//这就是那个bug的原因，因为这里没有show,没有这个没有保存到.ke里
		QSize size(1278, 732);
		_keWidget->OnPageSizeChange(size);
		_banTable->UpdateBans(_keWidget);
		_actionTable->UpdateActions(_keWidget);
	}
}

void MainWindow::OnSaveTriggered()
{
	QString path = QFileDialog::getSaveFileName(this, tr("保存文件"), ".", tr("课件文件(*.ke)"));
	if(!path.isEmpty())
	{
		if(_keWidget != NULL)
		{
			_keWidget->SaveDocument(path);
		}
	}
}
