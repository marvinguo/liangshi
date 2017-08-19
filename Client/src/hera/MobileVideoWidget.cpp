#include "MobileVideoWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

MobileVideoWidget::MobileVideoWidget(QWidget *parent):
QDialog(parent)
{
	setWindowIcon(QIcon(":/App/live"));
	//setWindowTitle(title);
	Qt::WindowFlags flags = windowFlags();
	flags = flags & (~Qt::WindowContextHelpButtonHint);
	flags = flags | Qt::WindowStaysOnTopHint | Qt::WindowMinimizeButtonHint;
	flags = flags & (~Qt::WindowCloseButtonHint);
	setWindowFlags(flags);

	setFixedSize(400, 400);

	QDesktopWidget *desktop = QApplication::desktop();
	move(desktop->width() - 403, desktop->height() -403);

	//我觉得做个timer吧，统一更新
	_timer = new QTimer(this);
	connect(_timer, &QTimer::timeout, this, &MobileVideoWidget::TimerUpdate);
	_timer->start(100);
}

MobileVideoWidget::~MobileVideoWidget()
{
}

void MobileVideoWidget::OnLocalVideoImageUpdate(int width, int height, unsigned char* data)
{
	_localImage = QImage(data, width, height, QImage::Format_ARGB32);
}

void MobileVideoWidget::OnRemotelVideoImageUpdate(int width, int height, unsigned char* data)
{
	_remoteImage = QImage(data, width, height, QImage::Format_ARGB32);
}

QPixmap MobileVideoWidget::GetPixmap()
{
	QPixmap localPixmap = QPixmap::fromImage(_localImage);
	QPixmap pixmap = localPixmap.copy();
	return pixmap;
}

void MobileVideoWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	Draw(&painter);
	QWidget::paintEvent(e);
}

void MobileVideoWidget::Draw(QPainter* painter)
{
	QMutexLocker lock(&_drawMutex);
	if(!_remoteImage.isNull())
	{
		QPixmap remotePixmap = QPixmap::fromImage(_remoteImage);
		if(!_localImage.isNull())
		{
			QPixmap localPixmap = QPixmap::fromImage(_localImage);
			QRect r = rect();
			painter->drawPixmap(0, 0, r.width(), r.height(), remotePixmap);

			int width = 150;
			int height = 150;

			int x = r.width() - width;
			int y = 0;
			painter->drawPixmap(x, y, width, height, localPixmap);
		}
	}
}

void MobileVideoWidget::TimerUpdate()
{
	update();
}