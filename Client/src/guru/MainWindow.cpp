#include "MainWindow.h"
#include "ConvertProgress.h"
#include "../hera/KeFile.h"
#include "../common/FileUtil.h"
#include "../hera/ChineseMessageBox.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow():
QMainWindow(NULL),
_playerWidget(NULL),
_keTime(new KeTime(this))
{
	setWindowIcon(QIcon(":/App/guru"));

	CreateActions();
	CreateToolBar();
	CreateDock();
}

MainWindow::~MainWindow()
{
	if(_playerWidget != NULL)
	{
		delete _playerWidget;
		_playerWidget = NULL;
	}
}

void MainWindow::CreateActions()
{
	_openAction = new QAction(tr("打开"), this);
	_openAction->setShortcuts(QKeySequence::Open);
	//_openAction->setStatusTip(tr("Create a new file"));
	connect(_openAction, &QAction::triggered, this, &MainWindow::OnOpenTriggered);

	_convertAction = new QAction(tr("转换"), this);
	connect(_convertAction, &QAction::triggered, this, &MainWindow::OnConvertTriggered);
}

void MainWindow::CreateDock()
{
}

void MainWindow::CreateToolBar()
{
	_toolBar = addToolBar(tr("标准"));
	_toolBar->addAction(_openAction);
	_toolBar->addAction(_convertAction);
}

void MainWindow::OnOpenTriggered()
{
	QString path = QFileDialog::getOpenFileName(this, QObject::tr("打开课件文件"), ".", QObject::tr("课件文件(*.ke)"));
	//QString path = "s:/0.ke";
	
	if(!path.isEmpty())
	{
		if(_playerWidget != NULL)
			delete _playerWidget;
		_playerWidget = KeFile::LoadKeAsPlayerWidget(path);
		_playerWidget->_keTime = _keTime;
	}
}

void MainWindow::OnConvertTriggered()
{
	QString path = QFileDialog::getSaveFileName(this, tr("保存文件"), ".", tr("课件文件(*.mp4)"));
	//QString path = "s:/0.mp4";
	if(!path.isEmpty())
	{
		if(_playerWidget != NULL)
		{
			bool ok;
			Qt::WindowFlags flags = Qt::Dialog | Qt::WindowCloseButtonHint;
			flags = flags & (~Qt::WindowContextHelpButtonHint);
			QString width = QInputDialog::getText(this, tr("输入参数"), tr("视频宽:"), QLineEdit::Normal, "1280", &ok, flags);
			QString height = QInputDialog::getText(this, tr("输入参数"), tr("视频高:"), QLineEdit::Normal, "800", &ok, flags);

			ConvertProgress progress(_playerWidget, width.toInt(), height.toInt(), this);
			progress.ShowModal();

			QString src = QCoreApplication::applicationDirPath() + "/ok.mp4";
			FileUtil::CopyFile(src, path);
			ChineseMessageBox::Show(QObject::tr("转换完成"), QObject::tr("完成"), ChineseMessageBox::OK, ChineseMessageBox::Information);
			//QFile::remove(dest);
		}
	}
}
