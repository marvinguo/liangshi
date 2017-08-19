#pragma once

#include <string>
#include <vector>
using namespace std;

#include <qtGui/QVector2D>
#include <qtGui/QPainterPath>

//本质来讲，我需要自己定就基础point类，不依赖于skia或是Qt
vector<QVector2D> toVector(const vector<QVector2D>& inputPoints, double tolerance, double error);
QPainterPath toPath(const vector<QVector2D>& inputPoints);
