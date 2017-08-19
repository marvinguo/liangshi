#include "AndroidCamera.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

AndroidCamera::AndroidCamera():
QtSingleton<AndroidCamera>(this)
{
	qDebug() << "AndroidCamera::AndroidCamera";
	_timer = new QTimer(this);
	connect(_timer, &QTimer::timeout, this, &AndroidCamera::CheckReturnPixmap);
	_timer->stop();
	qDebug() << "end AndroidCamera::AndroidCamera";
}

AndroidCamera::~AndroidCamera()
{
}

void AndroidCamera::TakePhoto(bool needContinue)
{
#ifdef Q_OS_ANDROID
	qDebug() << "AndroidCamera::TakePhoto";
	_continue = needContinue;
	//2014.9.29,下面的startActivity找不到例子，不会使用
	/*
	//QAndroidJniObject activity =  QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
	QAndroidJniObject CameraActivity("com/liangshiyy/teach_mobile/CameraActivity", "()V");
	if(CameraActivity.isValid())
	{
		qDebug() << "CameraActivity.isValid()";
		QtAndroid::startActivity(CameraActivity, 12051978, this);
	}
	*/
	QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/bindings/QtActivity", "takePhoto", "()V");
	QString temp = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/connectImage.jpg";
	qDebug() << "AndroidCamera jni part image path:" << temp;
	QFile::remove(temp);
	_timer->start(200);
	qDebug() << "end AndroidCamera::TakePhoto";
#endif
}

#ifdef Q_OS_ANDROID
void AndroidCamera::handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject & data)
{

}
#endif


void AndroidCamera::CheckReturnPixmap()
{
	//不能在主线程里，不然锁死了
	QString temp = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/connectImage.jpg";
	qDebug() << "AndroidCamera jni part image path:" << temp;
	if(!QFile::exists(temp))
	{
		qDebug() << "AndroidCamera hasn't got pixmap";
		return;
	}

	qDebug() << "AndroidCamera detect pixmap";
/*
	//check whether file locked
	QFile file(temp);
	bool opened = file.open(QIODevice::WriteOnly);
	if(!opened)
	{
		qDebug() << "AndroidCamera not opened";
		return;
	}
	file.close();
*/
	qDebug() << "Ok, now try to load";
	QFileInfo info(temp);
	if(info.size() > 0)
	{
		QPixmap pixmap;
		if(pixmap.load(temp))
		{
			qDebug() << "pixmap load successful";
			Q_EMIT OnCameraPixmap(pixmap);
		}
		else
		{
			qDebug() << "pixmap load failed";
		}
	}
	QFile::remove(temp);
	_timer->stop();
	Q_EMIT OnNeedContinue(_continue);
}