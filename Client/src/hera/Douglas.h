#pragma once

#include <vector>
using namespace std;
#include <qtGui/QVector2D>

vector<QVector2D> DouglasPeuckerReduction(vector<QVector2D>& points, double tolerance);
static void DouglasPeuckerReduction(vector<QVector2D>& points, int firstPoint, int lastPoint, double tolerance, vector<int>& pointIndexsToKeep);
static double PerpendicularDistance(QVector2D point1, QVector2D point2, QVector2D point);
