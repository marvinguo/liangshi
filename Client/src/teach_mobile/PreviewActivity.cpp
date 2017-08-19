#include "PreviewActivity.h"
#include "../common/QHttpFileDownloader.h"
#include "../hera/KeFile.h"
#include "MainWindow.h"
#include "MenuButton.h"
#include "LocalSave.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

PreviewActivity::PreviewActivity(MainWindow* mainWindow):
MobileActivity(mainWindow),
_keTime(new KeTime(topLevelWidget())),
_playerWidget(NULL)
{
	setContentsMargins(0, 0, 0, 0);
	_mainLayout = new QVBoxLayout();

	_title = new ActivityTitlebar(TitlebarType_Player, tr(""));
	connect(_title, &ActivityTitlebar::OnBack, this, &PreviewActivity::OnGoBack);
	_mainLayout->addWidget(_title);

	PlayTitlebar* play = _title->GetPlayTitleBar();
	play->SetKeTime(_keTime);

	_viewerLayout = new QVBoxLayout();

	_mainLayout->addLayout(_viewerLayout);
	_mainLayout->setSpacing(0);
	_mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(_mainLayout);
}

PreviewActivity::~PreviewActivity()
{
	if(_playerWidget != NULL)
	{
		_viewerLayout->removeWidget(_playerWidget);
		//_playerWidget->deleteLater();
		delete _playerWidget;
		_playerWidget = NULL;
	}
}

void PreviewActivity::OnBackKey()
{
	OnGoBack();
}

void PreviewActivity::OnPause()
{
	//OnGoBack();
	_keTime->Pause();
}

void PreviewActivity::UpdateData(bool offline, QString tempFile)
{
	qDebug() << "PreviewActivity::UpdateData";
	_offlicePreview = offline;

	if(_playerWidget != NULL)
	{
		_viewerLayout->removeWidget(_playerWidget);
		//_playerWidget->deleteLater();
		delete _playerWidget;
		_playerWidget = NULL;
	}

	if(QFile::exists(tempFile))
	{	
		_tempFile = tempFile;

		_playerWidget = KeFile::LoadKeAsPlayerWidget(tempFile);

		_keTime->Reset();
		_playerWidget->_keTime = _keTime;
		connect(_keTime, &KeTime::OnStart, _playerWidget, &PlayerWidget::OnPlayTriggered);
		connect(_keTime, &KeTime::OnPause, _playerWidget, &PlayerWidget::OnPauseTriggered);
		connect(_keTime, &KeTime::OnReset, _playerWidget, &PlayerWidget::OnResetTriggered);

		connect(_title->GetPlayTitleBar(), &PlayTitlebar::OnGotoPosition, _playerWidget, &PlayerWidget::OnGotoPositionTriggered);
		_title->GetPlayTitleBar()->SetTotalTime(_playerWidget->GetTotalTime());
		_viewerLayout->addWidget(_playerWidget);

		_keTime->Start();
		//updateGeometry();
		Util::EnableAndroidLandscape();
	}
}

void PreviewActivity::OnGoBack()
{
	_keTime->Pause();

	if(_offlicePreview)
		_mainWindow->OnGotoOffline();
	else
	{
		QFile::remove(_tempFile);
		_mainWindow->OnGotoKe();
	}
	Util::DisableAndroidLanscape();
}

