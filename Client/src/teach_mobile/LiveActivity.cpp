#include "LiveActivity.h"
#include "MainWindow.h"
#include "MenuButton.h"
#include "LocalSave.h"
#include "../hera/ChineseMessageBox.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

LiveActivity::LiveActivity(MainWindow* mainWindow):
MobileActivity(mainWindow),
_liveWidget(NULL),
_classLive(true)
{
	_mainLayout = new QVBoxLayout();
	_title = new ActivityTitlebar(TitlebarType_Live, tr(""));
	connect(_title, &ActivityTitlebar::OnBack, this, &LiveActivity::OnGoBack);
	_mainLayout->addWidget(_title);

	_mainLayout->setSpacing(0);
	_mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(_mainLayout);
}

LiveActivity::~LiveActivity()
{
	if(_liveWidget != NULL)
	{
		_mainLayout->removeWidget(_liveWidget);
		//_liveWidget->deleteLater();
		delete _liveWidget;
		_liveWidget = NULL;
	}
}

void LiveActivity::OnBackKey()
{
	OnGoBack();
}

void LiveActivity::OnPause()
{
}

void LiveActivity::UpdateData(bool classLive, bool allTeachers, QString& session)
{
	Util::EnableAndroidLandscape();
	_classLive = classLive;
	if(_liveWidget != NULL)
	{
		_mainLayout->removeWidget(_liveWidget);
		//_liveWidget->deleteLater();
		delete _liveWidget;
		_liveWidget = NULL;
	}

	LiveTitlebar* live = _title->GetLiveTitleBar();
	if(allTeachers)
	{
		live->GetApplyButton()->setVisible(false);
		live->GetTakeButton()->setVisible(true);
	}
	else
	{
		live->GetApplyButton()->setVisible(true);
		live->GetTakeButton()->setVisible(false);
	}

	qDebug() << "new LiveWidget(session);";
	_liveWidget = new LiveWidget(session);
	_liveWidget->ReviseDrawTriggered(false);
	qDebug() << "new LiveWidget connect";
	connect(_title->GetLiveTitleBar(), &LiveTitlebar::OnLineColorChanged, _liveWidget, &LiveWidget::LineColorTriggered);
	connect(_title->GetLiveTitleBar(), &LiveTitlebar::OnApplyBoard, _liveWidget, &LiveWidget::OnApplyBoardTriggered);
	connect(_title->GetLiveTitleBar(), &LiveTitlebar::OnTakeBoard, _liveWidget, &LiveWidget::OnTakeBoardTriggered);
	connect(_title->GetLiveTitleBar(), &LiveTitlebar::OnGetPixmap, _liveWidget, &LiveWidget::OnInsertPixmapBackgroundTriggered);
	connect(_title->GetLiveTitleBar(), &LiveTitlebar::OnShowVideo, _liveWidget, &LiveWidget::OnVideoWidget);
	connect(_title->GetLiveTitleBar(), &LiveTitlebar::OnGetLocalCamera, _liveWidget, &LiveWidget::OnGetLocalCameraTriggered);
	_mainLayout->addWidget(_liveWidget);
}

void LiveActivity::OnGoBack()
{
	_liveWidget->OnVideoWidget(false);
	int rb = ChineseMessageBox::Show(tr("确定退出?"), tr("正在直播中，确定退出"), ChineseMessageBox::YesNo, ChineseMessageBox::Question);
	if(rb == QMessageBox::Yes)
	{
	}
	if(rb == QMessageBox::No)
	{
		return;
	}
	_mainLayout->removeWidget(_liveWidget);
	//_liveWidget->deleteLater();
	delete _liveWidget;
	_liveWidget = NULL;
	Util::DisableAndroidLanscape();
	if(_classLive)
	{
		_mainWindow->OnGotoClass();
	}
	else
	{
		_mainWindow->OnGotoTeaching();
	}
}
