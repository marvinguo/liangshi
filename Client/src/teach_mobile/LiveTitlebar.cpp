#include "LiveTitlebar.h"
#include "../hera/CameraWindow.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

LiveTitlebar::LiveTitlebar(QWidget* parent):
QWidget(NULL),
_showVideo(false)
{
	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->setSizeConstraint(QLayout::SetMinimumSize);

	_apply = new ActivityTitleButton(tr("申请黑板"));
	mainLayout->addWidget(_apply);
	connect(_apply, &ActivityTitleButton::clicked, this, &LiveTitlebar::OnApplyBoard);

	_take = new ActivityTitleButton(tr("收回黑板"));
	mainLayout->addWidget(_take);
	connect(_take, &ActivityTitleButton::clicked, this, &LiveTitlebar::OnTakeBoard);

	_lineColor = new ColorButton(tr("笔的颜色"));
	_lineColor->setText(tr("线色"));
	connect(_lineColor, &ColorButton::OnColorChanged, this, &LiveTitlebar::OnLineColorChanged);
	mainLayout->addWidget(_lineColor);

	_camera = new ActivityTitleButton(tr("照相"));
	connect(_camera, &ActivityTitleButton::clicked, this, &LiveTitlebar::OnCameraButtonTriggered);
	mainLayout->addWidget(_camera);

	_video = new ActivityTitleButton(tr("视频"));
	connect(_video, &ActivityTitleButton::clicked, this, &LiveTitlebar::OnVideoButtonTriggered);
	mainLayout->addWidget(_video);

	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(mainLayout);
	

#ifdef Q_OS_ANDROID	
    AndroidCamera* camera = AndroidCamera::Instance();
	connect(camera, &AndroidCamera::OnCameraPixmap, this, &LiveTitlebar::OnGetPixmap);
#else
	CameraWindow* camera = CameraWindow::Instance();
	connect(camera, &CameraWindow::OnCameraPixmap, this, &LiveTitlebar::OnGetPixmap);
#endif
}

void LiveTitlebar::OnCameraButtonTriggered()
{
	bool hasLocal = Q_EMIT OnGetLocalCamera();

	if(!hasLocal)
	{
#ifdef Q_OS_ANDROID
    AndroidCamera* camera = AndroidCamera::Instance();
    camera->TakePhoto(false);
#else
	CameraWindow* camera = CameraWindow::Instance();
	camera->ShowModal();
#endif
	}
}

void LiveTitlebar::OnVideoButtonTriggered()
{
	_showVideo = !_showVideo;
	Q_EMIT OnShowVideo(_showVideo);
}
