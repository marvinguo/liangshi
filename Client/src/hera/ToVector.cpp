#include "ToVector.h"
#include "Douglas.h"
#include "FitCurves.h"
#include <algorithm>
#include <list>
#include <cmath>
using namespace std;

#if (defined Q_OS_IOS) || (defined Q_OS_ANDROID)
	#define _isnan isnan
#endif

bool hasNAN(QVector2D& point)
{
	return _isnan(point.x()) != 0 || _isnan(point.y()) != 0;
}

vector<QVector2D> toVector(const vector<QVector2D>& inputPoints, double tolerance, double error)
{
	list<QVector2D> removeDuplicate;
	removeDuplicate.insert(removeDuplicate.begin(), inputPoints.begin(), inputPoints.end());
	removeDuplicate.unique();
	vector<QVector2D> vectorPoints;
	vectorPoints.insert(vectorPoints.begin(), removeDuplicate.begin(), removeDuplicate.end());

	vector<QVector2D> simplified;
	if(tolerance > 0)
		simplified = DouglasPeuckerReduction(vectorPoints, tolerance);
	else
		simplified = vectorPoints;

	vector<QVector2D> result;

	if(simplified.size() > 2)
	{
		result = FitCurve(simplified, error);
		result.insert(result.begin(), simplified[0]);
	}
	else
	{
		result = simplified;
	}
	//ºÏ≤È «∑Ò”–-1.#IND000
	//remove_if(result.begin(), result.end(), hasNAN);

	if(result.size() <= 1)
		result.clear();

	return result;
}

QPainterPath toPath(const vector<QVector2D>& inputPoints) 
{
	//size 1,4,7,10,13
	QPainterPath path;

	if(inputPoints.size() == 2)
	{
		path.moveTo(inputPoints[0].toPointF());
		path.lineTo(inputPoints[1].toPointF());
		return path;
	}


	int loop = ((int)inputPoints.size() -1) / 3;
	if(loop <= 0)
		return path;

	path.moveTo(inputPoints[0].toPointF());
	for(int i = 0; i < loop; i++)
	{
		QVector2D controlPoint1 = inputPoints[i*3 + 1];
		QVector2D controlPoint2 = inputPoints[i*3 +2];
		QVector2D endPoint = inputPoints[i*3 +3];
		path.cubicTo(controlPoint1.toPointF(), controlPoint2.toPointF(), endPoint.toPointF());
	}
	return path;
}
