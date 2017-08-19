#include "PlayerTitleWidget.h"
#include "KeTime.h"
#include "HeraMainWidget.h"
#include "../common/FileUtil.h"
#include "KeWidget.h"
#include "TitleWidget.h"
#include "HackManager.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

PlayerTitleWidget::PlayerTitleWidget(NewableDocType type, KeTime* keTime, QWidget *parent):
QWidget(parent),
_keTime(keTime),
_paused(true),
//默认半小时，先这样吧，20141013, 改成1小时，看看
_totalTime(3600000),
_type(type),
_hera(NULL),
_isFullscreen(false)
{
	HackManager* hack = HackManager::Instance();

	_progress = new PlayerProgressBar();
	_progress->setMinimum(0);
	_progress->setMaximum(100);
	_progress->setValue(0);
	connect(_progress, &QSlider::valueChanged, this, &PlayerTitleWidget::OnSliderChangeTriggered);

	_play = new TwoStateImageButton(":/Icons/play", ":/Icons/pause");
	_play->setToolTip(tr("播放/暂停课件, 快捷键F5"));
	connect(_play, &TwoStateImageButton::clicked, this, &PlayerTitleWidget::OnPlayTriggered);

	QShortcut *f5 = new QShortcut(QKeySequence(Qt::Key_F5), this);
	f5->setContext(Qt::ApplicationShortcut);
	connect(f5, &QShortcut::activated, _play, &TwoStateImageButton::click);

	_full = new TitleButton(tr("全屏"));
	_full->setToolTip(tr("进入全屏模式,快捷键F11"));
	connect(_full, &TitleButton::clicked, this, &PlayerTitleWidget::OnFullscreenTriggered);

	QShortcut *f11 = new QShortcut(QKeySequence(Qt::Key_F11), this);
	f11->setContext(Qt::ApplicationShortcut);
	connect(f11, &QShortcut::activated, _full, &QPushButton::click);

	_test = new TitleButton(tr("查看试题"));
	_test->setToolTip(tr("查看试题"));
	connect(_test, &TitleButton::clicked, this, &PlayerTitleWidget::OnTestTriggered);

	_question = new TitleButton(tr("录制问题课件"));
	_question->setToolTip(tr("在当前板书下，录制课件"));
	connect(_question, &TitleButton::clicked, this, &PlayerTitleWidget::OnQuestionTriggered);

	_reRecord = new TitleButton(tr("修正录制"));
	_reRecord->setToolTip(tr("从当前时间开始修正录制"));
	connect(_reRecord, &TitleButton::clicked, this, &PlayerTitleWidget::OnReRecordTriggered);

	_currentTimeLabel = new QLabel();
	_currentTimeLabel->setFixedWidth(30);
	_currentTimeLabel->setText(tr("00:00"));

	_totalTimeLabel = new QLabel();
	_totalTimeLabel->setFixedWidth(30);
	_totalTimeLabel->setText(tr("00:00"));

	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->addWidget(_play);
	mainLayout->addWidget(_currentTimeLabel);
	mainLayout->addStretch();
	mainLayout->addWidget(_progress, 10, Qt::AlignLeft);
	mainLayout->addStretch();
	mainLayout->addWidget(_totalTimeLabel);
#ifdef QT_DEBUG
	//2014,1015,目前来讲，这个功能无望使用
	//mainLayout->addWidget(_test);
#endif
	mainLayout->addWidget(_full);

	if(_type != NewableDocType_PlayerPreview)
	{
		mainLayout->addWidget(_question);
		//超强功能

		if(hack->IsEnableKeSuperRevise())
			mainLayout->addWidget(_reRecord);
	}
	else
	{
		//todo,  有空的时候完成吧
		mainLayout->addWidget(_reRecord);
	}

	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	if(hack->IsEnableKeSuperRevise())
		setFixedWidth(650);
	else
		setFixedWidth(600);

	setFixedHeight(22);
	setLayout(mainLayout);
	setFocus();

	_question->setEnabled(false);
	_reRecord->setEnabled(false);
	Enable(false);
}

PlayerTitleWidget::~PlayerTitleWidget()
{
	if(_reRecord != NULL)
	{
		_reRecord->deleteLater();
		_reRecord = NULL;
	}
	if(_test != NULL)
	{
		_test->deleteLater();
		_test = NULL;
	}
	if(_question != NULL)
	{
		_question->deleteLater();
		_question = NULL;
	}
}

void PlayerTitleWidget::Init(QWidget* hera, PlayerWidget* playerWidget)
{
	_hera = hera;
	_playerWidget = playerWidget;
	connect(this, &PlayerTitleWidget::OnGotoPosition, _playerWidget, &PlayerWidget::OnGotoPositionTriggered);
}

void PlayerTitleWidget::Enable(bool able)
{
	_progress->setEnabled(able);
	_play->setEnabled(able);
	_test->setEnabled(able);
}

void PlayerTitleWidget::mouseDoubleClickEvent(QMouseEvent *)
{

}

void PlayerTitleWidget::OnPlayTriggered()
{
	KeTime* keTime = _keTime;
	//当前状态
	if(_paused)
	{
		keTime->Start();
	}
	else
	{
		keTime->Pause();
	}
}

void PlayerTitleWidget::OnFullscreenTriggered()
{
	if(_hera != NULL)
	{
		if(_isFullscreen)
		{
			if(_heraParent != NULL)
			{
				_heraParent->topLevelWidget()->show();
			}
			_hera->setParent(_heraParent);
			_hera->showNormal();
			_hera->resize(_oldSize);
			_full->setText(tr("全屏"));
			_full->setToolTip(tr("进入全屏模式"));
		}
		else
		{
			_oldSize = _hera->size();
			_heraParent = _hera->parentWidget();
			if(_heraParent != NULL)
			{
				_heraParent->topLevelWidget()->hide();
			}
			QThread::msleep(200);
			_hera->setParent(NULL);
			_hera->showFullScreen();
			_full->setText(tr("退出"));
			_full->setToolTip(tr("退出全屏模式"));

		}
	}
	_isFullscreen = !_isFullscreen;
}

void PlayerTitleWidget::OnTestTriggered()
{

}

void PlayerTitleWidget::OnStartTriggered()
{
	_question->setEnabled(false);
	_reRecord->setEnabled(false);
	_paused = false;
}

void PlayerTitleWidget::OnPauseTriggered()
{
	_question->setEnabled(true);
	_reRecord->setEnabled(true);
	_paused = true;
}

void PlayerTitleWidget::OnResetTriggered()
{
	_play->SetChecked(false);
}

void PlayerTitleWidget::OnSliderChangeTriggered(int value)
{
	double position = (double)value / 100;
	Q_EMIT OnGotoPosition(position);

	//这个地方主动更新一次时间
	OnTimeUpdateTriggered(position * _totalTime);
	if(_keTime->IsPausing())
	{
		_question->setEnabled(true);
		_reRecord->setEnabled(true);
	}
}

void PlayerTitleWidget::UpdateProgressPercent(double value)
{
	int position = value * 100;
	//主动update, 不能收到SliderChange
	disconnect(_progress, &QSlider::valueChanged, this, &PlayerTitleWidget::OnSliderChangeTriggered);
	_progress->setValue(position);
	connect(_progress, &QSlider::valueChanged, this, &PlayerTitleWidget::OnSliderChangeTriggered);
}

void PlayerTitleWidget::OnTimeUpdateTriggered(int time)
{
	KeTime* keTime = _keTime;
	if(time <= _totalTime)
	{
		QDateTime t1 = QDateTime::fromMSecsSinceEpoch(time);
		//t1.setTimeSpec(Qt::OffsetFromUTC);
		QDateTime t2 = t1.toUTC();
		//_time->setText(t2.toString("hh:mm:ss"));
		_currentTimeLabel->setText(t2.toString("mm:ss"));
		UpdateProgressPercent(double(time)/_totalTime);
	}
	else
	{
		UpdateProgressPercent(0);
		_currentTimeLabel->setText(tr("00:00"));
		keTime->Pause();
		keTime->Reset();
	}
}

void PlayerTitleWidget::SetTotalTime(int time)
{
	_totalTime = time;
	QDateTime t1 = QDateTime::fromMSecsSinceEpoch(time);
	QDateTime t2 = t1.toUTC();
	_totalTimeLabel->setText(t2.toString("mm:ss"));
}

void PlayerTitleWidget::OnQuestionTriggered()
{
	QWidget* window = topLevelWidget();
	if(window != NULL)
	{
		//window->hide();
		//LokiBuffer buffer = _playerWidget->GetMsgPackData();
		QPixmap background = _playerWidget->GetBackground();
		int time = _keTime->GetAnimatedTime();
		QDateTime t1 = QDateTime::fromMSecsSinceEpoch(time);
		QDateTime t2 = t1.toUTC();
		QString title = QString(tr("制作课件从%1").arg(t2.toString("mm:ss")));
		HeraMainWidget* hera = HeraMainWidget::BeginKeFromLokiBuffer(NewableDocType_KeEmbedded, background, title);
		if(hera != NULL)
		{
			//connect(hera, SIGNAL(OnClosed()), window, SLOT(OnShowTriggered()));
			hera->ShowModal();
		}
	}
}

void PlayerTitleWidget::OnReRecordTriggered()
{
	//抽取page, 
	//找到当前time, 生成
	int time = _keTime->GetAnimatedTime();
	HeraMainWidget* heraWidget = new HeraMainWidget(NewableDocType_Ke);
	//这个heraWidget需要加入到player hidden中，想办法


	//2014.1007 hades完成
	heraWidget->_title->GetKeWidget()->OnTimeUpdateTriggered(time);
	KeWidget* keWidget = (KeWidget*)(heraWidget->_mainDoc);
	int position = _playerWidget->_hades->GetPlayFilePosition(time);
	QString hadesFile = FileUtil::GetTempFolder() + QUuid::createUuid().toString();
	FileUtil::GetFilePart(_playerWidget->_hadesFile, hadesFile, position);
	keWidget->InitHadesFromExistingFile(hadesFile, position, time);

	//delete first page

	keWidget->_allPages.clear();
	keWidget->_mainLayout->removeWidget(keWidget->_currentPage);
	delete keWidget->_currentPage;
	keWidget->_currentPage = NULL;

	//终于找到赵老师说的问题了，以前的黑板是init了，只有switch的这块是play的，这里有有问题了
	//用第一个算法吧
	_playerWidget->GotoTime(time);
	//当前的每个page正好是想要的那个
	//1 page, 2 action
	for(auto i = _playerWidget->_allPages.begin(); i != _playerWidget->_allPages.end(); ++i)
	{
		//_allPages
		//利用messagepack来生成BanBaseWidget
		PlayerPage* page = i->second;

		//这个地方已经Goto time了，所以用drawing里的element?
		LokiBuffer buffer = page->_viewer->GetMsgPackData();
		BanBaseWidget* banPage = new BanBaseWidget(false);
		banPage->_canEditInside = false;
		banPage->SetMsgPackData(buffer);

		keWidget->AddPage(banPage, false);
	}

	int maxTime = 0;
	keWidget->_actions.clear();
	for(vector<AnimationAction*>::iterator i = _playerWidget->_actions.begin(); i != _playerWidget->_actions.end(); ++i)
	{
		AnimationAction* action = *i;
		int actionTime = action->GetTime();
		if(actionTime <= time)
		{
			AnimationAction* a = action->CreateFromThis();

			if(actionTime > maxTime)
				maxTime = actionTime;
			keWidget->_actions.push_back(a);
		}
	}

	//设置当前页
	QString uuid = _playerWidget->_currentPage->_viewer->GetObjectUuid();

	int j = 0;
	for(vector<BanBaseWidget*>::iterator i = keWidget->_allPages.begin(); i != keWidget->_allPages.end(); ++i, ++j)
	{
		BanBaseWidget* banWidget = *i;
		if(banWidget->GetObjectUuid() == uuid)
		{
			//这里需要设置maxTime,有一个垃圾技巧
			//keWidget->_keTime->SetAnimatedTime(maxTime+1);
			//keWidget->OnSelectPageTriggered(j);
			//keWidget->_keTime->SetAnimatedTime(time);

			//这个地方还是有bug, switch board不能随便加
			//因为之前已经有switch board了，所以这里不能发action
			keWidget->_currentPage = keWidget->_allPages[j];
			keWidget->_currentPage->SetMode(keWidget->_cachedMode);
			keWidget->_currentPage->LineWidthChanged(keWidget->_cachedWidth);
			keWidget->_currentPage->LineColorChanged(keWidget->_cachedColor);
			keWidget->_mainLayout->setCurrentWidget(keWidget->_currentPage);
		}
	}
	heraWidget->ShowModal();
}