#include <cmath>
#include <algorithm>
#include "Douglas.h"
//道格拉斯-普克抽稀算法

vector<QVector2D> DouglasPeuckerReduction(vector<QVector2D>& points, double tolerance)
{
	if(points.size() < 3)
		return points;

	int firstPoint = 0;
	int lastPoint = points.size() - 1;
	vector<int> pointIndexsToKeep;

	//Add the first and last index to the keepers
	pointIndexsToKeep.push_back(firstPoint);
	pointIndexsToKeep.push_back(lastPoint);

	//The first and the last point cannot be the same
	while(points[firstPoint] == points[lastPoint])
	{
		lastPoint--;
	}

	DouglasPeuckerReduction(points, firstPoint, lastPoint, tolerance, pointIndexsToKeep);
	vector<QVector2D> returnPoints;
	//测试过， .net sort, stl sort 都是升序
	sort(pointIndexsToKeep.begin(), pointIndexsToKeep.end());
	for(int index = 0; index <pointIndexsToKeep.size(); index++)
	{
		returnPoints.push_back(points[pointIndexsToKeep[index]]);
	}
	return returnPoints;
}

static void DouglasPeuckerReduction(vector<QVector2D>& points, int firstPoint, int lastPoint, double tolerance, vector<int>& pointIndexsToKeep)
{
	double maxDistance = 0;
	int indexFarthest = 0;

	for(int index = firstPoint; index < lastPoint; index++)
	{
		double distance = PerpendicularDistance(points[firstPoint], points[lastPoint], points[index]);
		if(distance > maxDistance)
		{
			maxDistance = distance;
			indexFarthest = index;
		}
	}

	if(maxDistance > tolerance && indexFarthest != 0)
	{
		//Add the largest point that exceeds the tolerance
		pointIndexsToKeep.push_back(indexFarthest);

		DouglasPeuckerReduction(points, firstPoint, indexFarthest, tolerance, pointIndexsToKeep);
		DouglasPeuckerReduction(points, indexFarthest, lastPoint, tolerance, pointIndexsToKeep);
	}
}

static double PerpendicularDistance(QVector2D point1, QVector2D point2, QVector2D point)
{
	double area = abs(0.5 * (point1.x() * point2.y() + point2.x() * point.y() + point.x() * point1.y() - point2.x() * point1.y() - point.x() * point2.y() - point1.x() * point.y()));
	double bottom = sqrt(pow((double)(point1.x() - point2.x()), 2) + pow((double)(point1.y() - point2.y()), 2));
	double height = area / bottom * 2;
	return height;
}