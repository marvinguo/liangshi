#pragma once

#include "../common/Qt.h"
#include "../common/library.h"
#include "../common/QtSingleton.h"

#ifdef Q_OS_WIN32
class LIANGSHIEXPORT CameraWindow : public QDialog, public QtSingleton<CameraWindow>
{
	Q_OBJECT
public:
	CameraWindow();
	virtual ~CameraWindow();

public:
	void ShowModal();

private:
	void closeEvent(QCloseEvent *event);

Q_SIGNALS:
	void OnCameraPixmap(QPixmap& pixmap);

private Q_SLOTS:
	void CapturedImageTriggered(int requestId, const QImage& image);
	void OnOKTriggered();
	void OnCancelTriggered();

private:
	QCamera* _camera;
	QCameraViewfinder* _viewfinder;
	QCameraImageCapture* _imageCapture;
};

#endif