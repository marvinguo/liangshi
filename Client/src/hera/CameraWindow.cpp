#include "CameraWindow.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

#ifdef Q_OS_WIN32
CameraWindow::CameraWindow():
QDialog(NULL),
QtSingleton<CameraWindow>(this)
{
	setWindowIcon(QIcon(":/App/collect"));
	setWindowTitle(QObject::tr("摄像头照相"));
	Qt::WindowFlags flags = windowFlags();
	flags = flags & (~Qt::WindowContextHelpButtonHint);
	setWindowFlags(flags);
	setModal(true);

	_camera = new QCamera(this);
	_viewfinder = new QCameraViewfinder(this);
	_imageCapture = new QCameraImageCapture(_camera, this);
	
	_camera->setViewfinder(_viewfinder);
	connect(_imageCapture, &QCameraImageCapture::imageCaptured, this, &CameraWindow::CapturedImageTriggered);

	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addWidget(_viewfinder);
	QHBoxLayout* hBox = new QHBoxLayout();
	QPushButton* ok = new QPushButton(tr("确定"));
	connect(ok, &QPushButton::clicked, this, &CameraWindow::OnOKTriggered);
	QPushButton* cancel = new QPushButton(tr("取消"));
	connect(cancel, &QPushButton::clicked, this, &CameraWindow::OnCancelTriggered);

	hBox->addWidget(ok);
	hBox->addWidget(cancel);
	mainLayout->addLayout(hBox);
	setLayout(mainLayout);

	setMinimumHeight(400);
	setMinimumWidth(600);
}

CameraWindow::~CameraWindow()
{
	if(_camera != NULL)
	{
		_camera->deleteLater();
		_camera = NULL;
	}
	if(_viewfinder != NULL)
	{
		_viewfinder->deleteLater();
		_viewfinder = NULL;
	}
	if(_imageCapture != NULL)
	{
		_imageCapture->deleteLater();
		_imageCapture = NULL;
	}
}

void CameraWindow::ShowModal()
{
	_camera->start();
	exec();
}

void CameraWindow::closeEvent(QCloseEvent *event)
{
	Q_UNUSED(event);
	//5.3.2这里要hang,为什么？？？
	_camera->stop();
}

void CameraWindow::CapturedImageTriggered(int requestId, const QImage& image)
{
	Q_UNUSED(requestId);
	//啥时候取消镜面？
	QImage a1 = image.mirrored();
	QTransform rot;
	rot.rotate(180);
	QImage a2 = a1.transformed(rot);


	//这里考虑手机的情况
#if QT_VERSION >= 0x050300
	QImage scaledImage = image.scaled(_viewfinder->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
#else
	QImage scaledImage = a2.scaled(_viewfinder->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
#endif
	QPixmap pixmap = QPixmap::fromImage(scaledImage);
	Q_EMIT OnCameraPixmap(pixmap);
	QTimer::singleShot(200, this, SLOT(close()));
}

void CameraWindow::OnOKTriggered()
{
	_imageCapture->capture();
}

void CameraWindow::OnCancelTriggered()
{
	close();
}

#endif