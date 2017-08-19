#pragma once

#include "../common/Qt.h"
#include "../common/library.h"

class MobileVideoWidget : public QDialog, public sigslot_liangshi::has_slots<>
{
	Q_OBJECT
public:
	MobileVideoWidget(QWidget *parent = 0);
	virtual ~MobileVideoWidget();
	
	void OnLocalVideoImageUpdate(int width, int height, unsigned char* data);
	void OnRemotelVideoImageUpdate(int width, int height, unsigned char* data);
	QPixmap GetPixmap();

private Q_SLOTS:
	void TimerUpdate();

private:
	virtual void paintEvent(QPaintEvent *e);

private:
	void Draw(QPainter* painter);

private:
	QMutex _drawMutex;
	QImage _remoteImage;
	QImage _localImage;
	QTimer* _timer;
};

