#pragma once

#include "Qt.h"
#include "library.h"
#include "SPoint.h"
#include "../../../SharedCode/LokiBuffer.h"

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

class Util
{
public:
	static vector<QVector2D> QPoint2QVector2D(vector<QPoint>& points);
	static vector<SPoint> Vector2D2Point(vector<QVector2D>& points);
	static vector<QVector2D> Point2Vector2D(vector<SPoint>& points);
	static QPoint Point2QPoint(SPoint& p);
	static QPoint GetPerfectPoint(QPoint start, QPoint end);
	static int RandomInteger(int low, int high);
	static LokiBuffer GenerateRandomLokiBuffer();
	static void ClearLayout(QLayout* layout, bool deleteWidgets = true, bool clearChild = false);
	static bool IsMathML(QString& xml);
	static QString GetAndroidDeviceID();
	static QString GetAndroidKePath();
	static QString GetAndroidWritablePath();
	static void OpenAndroidWebsite(QString path);
	static void EnableAndroidLandscape();
	static void DisableAndroidLanscape();
};

