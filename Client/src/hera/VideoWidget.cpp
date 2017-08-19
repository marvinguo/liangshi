#include "VideoWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

VideoWidget::VideoWidget(QString title, QWidget *parent):
QDialog(parent)
{
	setWindowIcon(QIcon(":/App/live"));
	setWindowTitle(title);
	Qt::WindowFlags flags = windowFlags();
	flags = flags & (~Qt::WindowContextHelpButtonHint);
	flags = flags | Qt::WindowStaysOnTopHint | Qt::WindowMinimizeButtonHint;
	flags = flags & (~Qt::WindowCloseButtonHint);
	setWindowFlags(flags);
}

VideoWidget::~VideoWidget()
{
}

void VideoWidget::OnVideoImageUpdate(int width, int height, unsigned char* data)
{
	_image = QImage(data, width, height, QImage::Format_ARGB32);
	update();
}

QPixmap VideoWidget::GetPixmap()
{
	QPixmap pixmap(_aSize.width(), _aSize.height());
	QPainter painter(&pixmap);
	Draw(&painter);
	return pixmap;
}

void VideoWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	Draw(&painter);
	QWidget::paintEvent(e);
}

void VideoWidget::resizeEvent(QResizeEvent *e)
{
	_aSize = e->size();
	QWidget::resizeEvent(e);
}

void VideoWidget::Draw(QPainter* painter)
{
	QPixmap pixmap = QPixmap::fromImage(_image);
	QRect r = rect();
	painter->drawPixmap(0, 0, r.width(), r.height(), pixmap);
}