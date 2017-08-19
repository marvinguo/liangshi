#pragma once

#include "../common/Qt.h"
#include "../common/QtSingleton.h"

class AndroidCamera : public QObject,
	public QtSingleton<AndroidCamera>
#ifdef Q_OS_ANDROID
		, public QAndroidActivityResultReceiver
#endif
{
	Q_OBJECT
public:
	AndroidCamera();
	virtual ~AndroidCamera();
	void TakePhoto(bool needContinue);

private:
#ifdef Q_OS_ANDROID
	virtual void	handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject & data);
#endif

Q_SIGNALS:
	void OnCameraPixmap(QPixmap& pixmap);
	void OnNeedContinue(bool needContinue);

private Q_SLOTS:
	void CheckReturnPixmap();

private:
	QTimer* _timer;
	bool _continue;
};

