#pragma once

#include "../common/Qt.h"
#include "../common/library.h"
#include "../common/QtSingleton.h"

class LIANGSHIEXPORT ScreenshotWindow : public QDialog, public QtSingleton<ScreenshotWindow>
{
	Q_OBJECT
public:
	ScreenshotWindow();
	virtual ~ScreenshotWindow();
	void ShowModal();

private:
	virtual void paintEvent(QPaintEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void closeEvent(QCloseEvent * event);

Q_SIGNALS:
	void OnScreenPixmap(QPixmap& pixmap);
	void OnEnd();

private Q_SLOTS:
	void OnOKTriggered();

private:
	void PaintPixmap(QPixmap &pixmap);

private:
	QPixmap _fullPixmap;
	QPixmap _backgroundPixmap;
	QPixmap _drawingPixmap;
	QRect _captureRect;
	QToolBar *_toolbar;
	bool _isDrawing;
	QPoint _startPoint;
	QPoint _endPoint;
};

