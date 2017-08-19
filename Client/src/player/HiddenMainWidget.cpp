#include "HiddenMainWidget.h"
#include "../common/config.h"
#include "../hera/ChineseMessageBox.h"
#include "../hera/HeraDocManager.h"
#include "../hera/HackManager.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

HiddenMainWidget::HiddenMainWidget(QWidget *parent):
QWidget(parent),
_hera(NULL),
_preview(false)
{
	HeraDocManager* manager = HeraDocManager::Instance();
	connect(manager, &HeraDocManager::OnHeraCountChanged, this, &HiddenMainWidget::OnHeraCountChangeTriggered);

	//detect shift
	Qt::KeyboardModifiers modifiers  = QApplication::queryKeyboardModifiers ();
	if(modifiers.testFlag(Qt::ControlModifier))
	{
		HackManager* hack = HackManager::Instance();
		hack->EnterSuperMode();
	}

	QString filepath = "";
	QStringList arguments = QCoreApplication::arguments();
	for(int i = 1; i < arguments.size(); ++i)
	{
		QString arg = arguments[i];

		if(arg == tr("-preview"))
		{
			_preview = true;
			continue;
		}

		filepath = arg;
	}

	if(filepath != "")
	{
		if(_preview)
			OpenPreview(filepath);
		else
			OpenFileTriggered(filepath);
	}
	else
	{
		_hera = new HeraMainWidget(NewableDocType_Player);
		connect(_hera, &HeraMainWidget::OnOpenKeFile, this, &HiddenMainWidget::OpenFileTriggered);
		ResizeHera();
		_hera->show();
	}
}

HiddenMainWidget::~HiddenMainWidget()
{

}

void HiddenMainWidget::OpenPreview(QString filepath)
{
	if(_hera != NULL)
	{
		_hera->deleteLater();
		_hera = NULL;
	}

	_hera = HeraMainWidget::LoadKeFile(filepath, false, true);
	connect(_hera, &HeraMainWidget::OnOpenKeFile, this, &HiddenMainWidget::OpenFileTriggered);
	ResizeHera();
	_hera->show();

	//load完，删除源文件
	QFile::remove(filepath);
}


void HiddenMainWidget::OpenFileTriggered(QString filepath)
{

	HeraMainWidget* hera = HeraMainWidget::LoadKeFile(filepath);

	if(hera == NULL)
	{
		ChineseMessageBox::Show(tr("错误"), tr("文件不能识别为课件文件"), ChineseMessageBox::OK, ChineseMessageBox::Error);
		return;
	}

	if(_hera != NULL)
	{
		_hera->deleteLater();
		_hera = NULL;
	}
	_hera = hera;
	connect(_hera, &HeraMainWidget::OnOpenKeFile, this, &HiddenMainWidget::OpenFileTriggered);
	ResizeHera();
	_hera->show();
}

void HiddenMainWidget::ResizeHera()
{
	int width = BOARD_WIDTH;
	int height = BOARD_HEIGHT;
	QDesktopWidget *desktop = QApplication::desktop();
	if(desktop->availableGeometry().width() < width)
		width = desktop->availableGeometry().width();
	if(desktop->availableGeometry().height() < height)
		height = desktop->availableGeometry().height();
	_hera->resize(width, height);
}

void HiddenMainWidget::OnHeraCountChangeTriggered(int count)
{
	if(count == 0)
	{
		//qApp->quit();
		close();
	}
}

