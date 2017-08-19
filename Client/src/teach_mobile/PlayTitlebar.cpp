#include "PlayTitlebar.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

PlayTitlebar::PlayTitlebar(QWidget* parent):
QWidget(NULL),
_paused(true)
{
	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->setSizeConstraint(QLayout::SetMinimumSize);

	_play = new ActivityTitleButton(tr("播放"));
	mainLayout->addWidget(_play);
	connect(_play, &ActivityTitleButton::clicked, this, &PlayTitlebar::OnPlayButtonTriggered);
	
	_currentTimeLabel = new ActivityTitleLabel(tr("00:00"));
	mainLayout->addWidget(_currentTimeLabel, 1, Qt::AlignLeft);

	mainLayout->addStretch();

	_progress = new PlayerProgressBar();
	_progress->setMinimum(0);
	_progress->setMaximum(100);
	_progress->setValue(0);
	//_progress->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	connect(_progress, &QSlider::valueChanged, this, &PlayTitlebar::OnSliderChangeTriggered);
	mainLayout->addWidget(_progress, 10, Qt::AlignLeft);

	mainLayout->addStretch();

	_totalTimeLabel = new ActivityTitleLabel(tr("00:00"));
	mainLayout->addWidget(_totalTimeLabel, 1, Qt::AlignRight);
	
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(mainLayout);

	QDesktopWidget *desktop = QApplication::desktop();
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

#define TEST_WIDTH 400
#define TEST_HEIGHT 700

	int wholeWidth =1024;

#ifdef _WIN32
	wholeWidth = TEST_WIDTH;
	setMaximumWidth(TEST_WIDTH);
	//setMaximumHeight(TEST_HEIGHT);
#else
	wholeWidth = desktop->availableGeometry().width();
	setMaximumWidth(desktop->availableGeometry().width());
	//setMaximumHeight(desktop->availableGeometry().height());
#endif
	int width = wholeWidth - _play->width()- _currentTimeLabel->width() - _totalTimeLabel->width();
	//_progress->setMinimumWidth(width);
}

void PlayTitlebar::SetKeTime(KeTime* keTime)
{
	_keTime = keTime;
	connect(_keTime, &KeTime::OnStart, this, &PlayTitlebar::OnStartTriggered);
	connect(_keTime, &KeTime::OnPause, this, &PlayTitlebar::OnPauseTriggered);
	connect(keTime, &KeTime::OnUpdateCurrentTime, this, &PlayTitlebar::OnTimeUpdateTriggered);
}

void PlayTitlebar::OnPlayButtonTriggered()
{
	if(_paused)
	{
		_keTime->Start();
	}
	else
	{
		_keTime->Pause();
	}
}

void PlayTitlebar::OnSliderChangeTriggered(int value)
{
	double position = (double)value / 100;
	Q_EMIT OnGotoPosition(position);

	//这个地方主动更新一次时间
	OnTimeUpdateTriggered(position * _totalTime);
}

void PlayTitlebar::OnStartTriggered()
{
	_play->setText(tr("暂停"));
	_paused = false;
}

void PlayTitlebar::OnPauseTriggered()
{
	_play->setText(tr("播放"));
	_paused = true;
}

void PlayTitlebar::SetTotalTime(int time)
{
	_totalTime = time;
	QDateTime t1 = QDateTime::fromMSecsSinceEpoch(time);
	QDateTime t2 = t1.toUTC();
	_totalTimeLabel->setText(t2.toString("mm:ss"));
}

void PlayTitlebar::OnTimeUpdateTriggered(int time)
{
	KeTime* keTime = _keTime;//KeRecordingTime::GetInstance(topLevelWidget());
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

void PlayTitlebar::UpdateProgressPercent(double value)
{
	int position = value * 100;
	//主动update, 不能收到SliderChange
	disconnect(_progress, &QSlider::valueChanged, this, &PlayTitlebar::OnSliderChangeTriggered);
	_progress->setValue(position);
	connect(_progress, &QSlider::valueChanged, this, &PlayTitlebar::OnSliderChangeTriggered);
}