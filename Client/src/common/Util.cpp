#include "Util.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

vector<QVector2D> Util::QPoint2QVector2D(vector<QPoint>& points)
{
	vector<QVector2D> v;

	for(vector<QPoint>::iterator i = points.begin(); i != points.end(); ++i)
		v.push_back(QVector2D(*i));

	return v;
}

vector<SPoint> Util::Vector2D2Point(vector<QVector2D>& points)
{
	vector<SPoint> v;

	for(vector<QVector2D>::iterator i = points.begin(); i != points.end(); ++i)
		v.push_back(SPoint(*i));

	return v;

}

vector<QVector2D> Util::Point2Vector2D(vector<SPoint>& points)
{
	vector<QVector2D> v;

	for(vector<SPoint>::iterator i = points.begin(); i != points.end(); ++i)
	{
		QVector2D p((*i)._x, (*i)._y);
		v.push_back(p);
	}
	return v;

}

QPoint Util::Point2QPoint(SPoint& p)
{
	return QPoint(p._x, p._y);
}

QPoint Util::GetPerfectPoint(QPoint start, QPoint end)
{
	int xDiff = end.x() - start.x();
	int yDiff = end.y() - start.y();

//	int length = qMin(qAbs(xDiff), qAbs(yDiff));
	int length = qMax(qAbs(xDiff), qAbs(yDiff));

	int x = sgn(xDiff) * length;
	int y = sgn(yDiff) * length;
	return QPoint(start.x() + x, start.y() + y);
}

int Util::RandomInteger(int low, int high)
{
	return qrand() % ((high + 1) - low) + low;
}

LokiBuffer Util::GenerateRandomLokiBuffer()
{
	QTime time = QTime::currentTime();
	qsrand((uint)time.msec());

	int size = RandomInteger(50,100);

	LokiBuffer buffer;
	for(int i = 0; i < size; i++)
	{
		char c = (char)RandomInteger(0, 255);
		buffer.AddChar(c);
	}
	return buffer;
}

void Util::ClearLayout(QLayout* layout, bool deleteWidgets, bool clearChild)
{
	while (QLayoutItem* item = layout->takeAt(0))
	{
		if(deleteWidgets)
		{
			if(QWidget* widget = item->widget())
				delete widget;
		}

		if(clearChild)
		{
			if(QLayout* childLayout = item->layout())
				ClearLayout(childLayout, deleteWidgets, clearChild);
		}
		delete item;
	}
}

bool Util::IsMathML(QString& xml)
{
	if(xml.size() > 5)
	{
		QString math = xml.mid(0, 5);
		return math == "<math";
	}
	return false;
}

QString Util::GetAndroidDeviceID()
{
#ifdef Q_OS_ANDROID
	QAndroidJniObject path = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/bindings/QtActivity", "getDeviceID", "()Ljava/lang/String;");
	return path.toString();
#else
	return "windows_test";
#endif
}

QString Util::GetAndroidKePath()
{
#ifdef Q_OS_ANDROID
	QAndroidJniObject path = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/bindings/QtActivity", "KePath", "()Ljava/lang/String;");
	QString s = path.toString();
	qDebug() << "Util::GetAndroidKePath from java:" << s;
	if(s.isEmpty())
		return QString(QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/");
	else
		return s;
#else
	return "";
#endif
}

QString Util::GetAndroidWritablePath()
{
#ifdef Q_OS_ANDROID
	QAndroidJniObject path = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/bindings/QtActivity", "getWritablePath", "()Ljava/lang/String;");
	QString s = path.toString();
	if(s.isEmpty())
		return QString(QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/");
	else
		return s;
#else
	return "";
#endif
}

/*
void Util::SetAndroidAlwaysLight(bool enable)
{
#ifdef Q_OS_ANDROID
	//这个有问题
	//QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/bindings/QtActivity", "alwaysLight", "(Z)V", enable);
#endif
}
*/

void Util::OpenAndroidWebsite(QString path)
{
	qDebug() << "c++ OpenAndroidWebsite";
#ifdef Q_OS_ANDROID
	QAndroidJniObject jtext = QAndroidJniObject::fromString(path);
	QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/bindings/QtActivity", "openURL", "(Ljava/lang/String;)V", jtext.object<jstring>());
#else
	QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));
#endif
	qDebug() << "c++ end OpenAndroidWebsite";
}

//这2个函数有问题，需要研究android的工作方式
void Util::EnableAndroidLandscape()
{
#ifdef Q_OS_ANDROID
	qDebug() << "c++ EnableAndroidLandscape";
	QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/bindings/QtActivity", "enableAndroidLandscape", "()V");
	qDebug() << "c++ end EnableAndroidLandscape";
#endif
}

void Util::DisableAndroidLanscape()
{
#ifdef Q_OS_ANDROID
	qDebug() << "c++ DisableAndroidLanscape";
	QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/bindings/QtActivity", "disableAndroidLandscape", "()V");
	qDebug() << "c++ end DisableAndroidLanscape";
#endif
}

