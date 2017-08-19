#include "PlayerActivity.h"
#include "../common/QHttpFileDownloader.h"
#include "../common/FileUtil.h"
#include "../hera/KeFile.h"
#include "MainWindow.h"
#include "MenuButton.h"
#include "LocalSave.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

PlayerActivity::PlayerActivity(MainWindow* mainWindow):
MobileActivity(mainWindow),
_keTime(new KeTime(topLevelWidget())),
_playerWidget(NULL)
{
	setContentsMargins(0, 0, 0, 0);
	_mainLayout = new QStackedLayout();

	_viewer = new QWidget();
	_viewerLayout = new QVBoxLayout();

	_title = new ActivityTitlebar(TitlebarType_Player, tr(""));
	connect(_title, &ActivityTitlebar::OnBack, this, &PlayerActivity::OnGoBack);
	_viewerLayout->addWidget(_title);

	PlayTitlebar* play = _title->GetPlayTitleBar();
	play->SetKeTime(_keTime);

	_viewerLayout->setSpacing(0);
	_viewerLayout->setContentsMargins(0, 0, 0, 0);
	_viewer->setLayout(_viewerLayout);

	_busy = new BusyIndicator(this);
	_busy->setVisible(false);
	//_busyLayout->addWidget(_busy);
	_busyLabel = new QLabel(tr("正在载入中, 请稍后..."), this);
	QFont f("Arial", 30, QFont::Bold);
	_busyLabel->setFont(f);
	_busyLabel->setStyleSheet("qproperty-alignment: AlignCenter;color:blue");
	//_label->setVisible(false);
	//_busyLayout->addWidget(_label);

	_mainLayout->addWidget(_busyLabel);
	_mainLayout->addWidget(_viewer);
	_mainLayout->setCurrentWidget(_busyLabel);

	setLayout(_mainLayout);
}

PlayerActivity::~PlayerActivity()
{
	if(_busy != NULL)
	{
		_busy->deleteLater();
		_busy = NULL;
	}

	if(_playerWidget != NULL)
	{
		_viewerLayout->removeWidget(_playerWidget);
		//_playerWidget->deleteLater();
		delete _playerWidget;
		_playerWidget = NULL;
	}
}

void PlayerActivity::OnBackKey()
{
	OnGoBack();
}

void PlayerActivity::OnPause()
{
	//OnGoBack();
	_keTime->Pause();
}

void PlayerActivity::UpdateData(bool classPlay, QString classTitle, KeDTO keDTO)
{
	//这个地方应该起菊花？？
	//_busy->setVisible(true);
	_mainLayout->setCurrentWidget(_busyLabel);
	//启新线程下载，主线程是菊花
	//QFuture<void> future = QtConcurrent::run(PlayerActivity::DownloadKe, url, this);

	if(_playerWidget != NULL)
	{
		_viewerLayout->removeWidget(_playerWidget);
		//_playerWidget->deleteLater();
		delete _playerWidget;
		_playerWidget = NULL;
	}
	_classPlay = classPlay;
	_classTitle = classTitle;
	_keDTO = keDTO;
	PlayerActivity::DownloadKe(keDTO.url, this);
}

void PlayerActivity::DownloadKe(QString url, PlayerActivity* activity)
{
	//在android手机上，重装一下，这个位置会抹掉
	QString local = FileUtil::GetKeCacheFolder() + url;
	QFileInfo info(local);
	if(activity->_keDTO.filesize == info.size() && info.exists())
	{
		activity->DownloadKeFileTriggered(local);
	}
	else
	{
		//1. 要不多线程，要不菊花

		QString remote = "/ke/" + url;
		QHttpFileDownloader* downloader = new QHttpFileDownloader("liangshi.oss-cn-hangzhou.aliyuncs.com", remote, local);
		//不同线程，怎么弄？试试
		//connect(downloader, &QHttpFileDownloader::DownloadFinished, activity, &PlayerActivity::DownloadKeFileTriggered, Qt::DirectConnection);
		connect(downloader, &QHttpFileDownloader::DownloadFinished, activity, &PlayerActivity::DownloadKeFileTriggered);
		downloader->Download();
	}
}

void PlayerActivity::DownloadKeFileTriggered(QString localFile)
{
	QHttpFileDownloader* downloader = qobject_cast<QHttpFileDownloader *>(sender());
	if(downloader != NULL)
		delete downloader;

	//为离线保存
	LocalSave save;
	save.AddKeFile(_classTitle, _keDTO);

	//_busy->setVisible(false);
	_mainLayout->setCurrentWidget(_viewer);
	_playerWidget = KeFile::LoadKeAsPlayerWidget(localFile);
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

void PlayerActivity::OnGoBack()
{
	_keTime->Pause();
	
	if(_classPlay)
	{
		_mainWindow->OnGotoClass();
	}
	else
	{
		_mainWindow->OnGotoTeaching();
	}
	Util::DisableAndroidLanscape();
}
