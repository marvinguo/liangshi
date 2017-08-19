#pragma once

#include "../common/Qt.h"
#include "../common/library.h"

class VideoWidget : public QDialog, public sigslot_liangshi::has_slots<>
{
	Q_OBJECT
public:
	VideoWidget(QString title, QWidget *parent = 0);
	virtual ~VideoWidget();
	
	void OnVideoImageUpdate(int width, int height, unsigned char* data);
	QPixmap GetPixmap();

private:
	virtual void paintEvent(QPaintEvent *e);
	virtual void resizeEvent(QResizeEvent *e);

private:
	void Draw(QPainter* painter);

private:
	QSize _aSize;
	QImage _image;
};

