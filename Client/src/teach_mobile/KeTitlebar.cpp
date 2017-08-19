#include "KeTitlebar.h"
#include "../hera/CameraWindow.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

KeTitlebar::KeTitlebar(QWidget* parent):
QWidget(NULL),
_paused(true)
{
	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->setSizeConstraint(QLayout::SetMinimumSize);

	_record = new ActivityTitleButton(tr("录制"));
	mainLayout->addWidget(_record);
	connect(_record, &ActivityTitleButton::clicked, this, &KeTitlebar::OnRecordButtonTriggered);

	_time = new ActivityTitleLabel(tr("00:00"));
	mainLayout->addWidget(_time);

	_lineColor = new ColorButton(tr("笔的颜色"));
	_lineColor->setText(tr("线色"));
	connect(_lineColor, &ColorButton::OnColorChanged, this, &KeTitlebar::OnLineColorChanged);
	mainLayout->addWidget(_lineColor);

	_camera = new ActivityTitleButton(tr("照相"));
	connect(_camera, &ActivityTitleButton::clicked, this, &KeTitlebar::OnCameraButtonTriggered);
	mainLayout->addWidget(_camera);

	_preview = new ActivityTitleButton(tr("预览"));
	connect(_preview, &ActivityTitleButton::clicked, this, &KeTitlebar::OnPreview);
	_preview->setDisabled(true);
	mainLayout->addWidget(_preview);

	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(mainLayout);

#ifdef Q_OS_ANDROID	
	AndroidCamera* camera = AndroidCamera::Instance();
	connect(camera, &AndroidCamera::OnCameraPixmap, this, &KeTitlebar::OnGetPixmap);
	connect(camera, &AndroidCamera::OnNeedContinue, this, &KeTitlebar::OnCameraContinue);
#else
	CameraWindow* camera = CameraWindow::Instance();
	connect(camera, &CameraWindow::OnCameraPixmap, this, &KeTitlebar::OnGetPixmap);
#endif
}

void KeTitlebar::SetKeTime(KeTime* keTime)
{
	_keTime = keTime;
	connect(_keTime, &KeTime::OnStart, this, &KeTitlebar::OnStartTriggered);
	connect(_keTime, &KeTime::OnPause, this, &KeTitlebar::OnPauseTriggered);
	connect(_keTime, &KeTime::OnUpdateCurrentTime, this, &KeTitlebar::OnTimeUpdateTriggered);
}

void KeTitlebar::OnStartTriggered()
{
	_record->setText(tr("暂停"));
	_paused = false;
}

void KeTitlebar::OnPauseTriggered()
{
	_record->setText(tr("录制"));
	_paused = true;
}

void KeTitlebar::OnRecordButtonTriggered()
{
	if(_paused)
	{
		_keTime->Start();
	}
	else
	{
		_keTime->Pause();
	}
	_preview->setDisabled(!_paused);
}

void KeTitlebar::OnCameraButtonTriggered()
{
	bool needContinue = !_keTime->IsPausing();
	_keTime->Pause();

#ifdef Q_OS_ANDROID
	AndroidCamera* camera = AndroidCamera::Instance();
	camera->TakePhoto(needContinue);
#else
	CameraWindow* camera = CameraWindow::Instance();
	camera->ShowModal();
	//对于android,这里不是模态的，所以会立即返回
	if(needContinue)
		_keTime->Start();
#endif
}

void KeTitlebar::OnCameraContinue(bool needContinue)
{
	if(needContinue)
		_keTime->Start();
}

void KeTitlebar::OnTimeUpdateTriggered(int time)
{
	KeTime* keTime = _keTime;

	{
		QDateTime t1 = QDateTime::fromMSecsSinceEpoch(time);
		//t1.setTimeSpec(Qt::OffsetFromUTC);
		QDateTime t2 = t1.toUTC();
		//_time->setText(t2.toString("hh:mm:ss"));
		_time->setText(t2.toString("mm:ss"));
	}
}