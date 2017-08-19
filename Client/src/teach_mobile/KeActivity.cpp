#include "KeActivity.h"
#include "MainWindow.h"
#include "MenuButton.h"
#include "LocalSave.h"
#include "../hera/ChineseMessageBox.h"
#include "../common/FileUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

KeActivity::KeActivity(MainWindow* mainWindow):
MobileActivity(mainWindow),
_keTime(new KeTime(topLevelWidget())),
_keWidget(NULL)
{
	_mainLayout = new QVBoxLayout();
	_title = new ActivityTitlebar(TitlebarType_Ke, tr(""));
	connect(_title, &ActivityTitlebar::OnBack, this, &KeActivity::OnGoBack);
	connect(_title->GetKeTitleBar(), &KeTitlebar::OnPreview, this, &KeActivity::OnPreviewTirggered);
	_mainLayout->addWidget(_title);

	KeTitlebar* ke = _title->GetKeTitleBar();
	ke->SetKeTime(_keTime);

	_mainLayout->setSpacing(0);
	_mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(_mainLayout);
}

KeActivity::~KeActivity()
{
	if(_keWidget != NULL)
	{
		_mainLayout->removeWidget(_keWidget);
		//_keWidget->deleteLater();
		delete _keWidget;
		_keWidget = NULL;
	}
}

void KeActivity::OnBackKey()
{
	OnGoBack();
}

void KeActivity::OnPause()
{
	_keTime->Pause();
}

void KeActivity::UpdateData(QString& id)
{
	if(_keWidget != NULL)
	{
		_mainLayout->removeWidget(_keWidget);
		//_keWidget->deleteLater();
		delete _keWidget;
		_keWidget = NULL;
	}

	_title->GetKeTitleBar()->OnTimeUpdateTriggered(0);

	_keWidget = new KeWidget(_keTime);
	_keWidget->ReviseDrawTriggered(false);
	_keTime->Reset();
	_keWidget->_keTime = _keTime;
	connect(_keTime, &KeTime::OnStart, _keWidget, &KeWidget::OnRecordTriggered);
	connect(_keTime, &KeTime::OnPause, _keWidget, &KeWidget::OnPauseTriggered);

	connect(_title->GetKeTitleBar(), &KeTitlebar::OnLineColorChanged, _keWidget, &KeWidget::LineColorTriggered);
	connect(_title->GetKeTitleBar(), &KeTitlebar::OnGetPixmap, _keWidget, &KeWidget::OnInsertPixmapBackgroundTriggered);

	_mainLayout->addWidget(_keWidget);
}

void KeActivity::OnGoBack()
{
	_keTime->Pause();
	if(_keWidget != NULL)
	{
		if(!_keWidget->CanQuit())
		{
			int rb = ChineseMessageBox::Show(tr("是否上传?"), tr("课件完成"), ChineseMessageBox::YesNoCancel, ChineseMessageBox::Question);
			if(rb == QMessageBox::Yes)
			{
			}
			if(rb == QMessageBox::No)
			{
			}

			if(rb == QMessageBox::Cancel)
			{
				return;
			}
		}
		_mainWindow->OnGotoClass();
	}
}

void KeActivity::OnPreviewTirggered()
{
	if(_keWidget != NULL)
	{
		QString temp = FileUtil::GetKeCacheFolder() + QUuid::createUuid().toString();;
		_keWidget->SaveAsDocument(temp);
		_mainWindow->OnGotoPreview(false, temp);
	}
}