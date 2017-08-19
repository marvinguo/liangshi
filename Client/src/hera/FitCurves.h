#pragma once

#include <vector>
using namespace std;
#include <qtGui/QVector2D>

vector<QVector2D> FitCurve(vector<QVector2D>& d, double error);
static void FitCubic(vector<QVector2D>& d, int first, int last, QVector2D tHat1, QVector2D tHat2, double error,vector<QVector2D>& result);
static QVector2D* GenerateBezier(vector<QVector2D>& d, int first, int last, double* uPrime, QVector2D& tHat1, QVector2D& tHat2);
static double* Reparameterize(vector<QVector2D>& d,int first,int last,double* u, QVector2D* bezCurve);
static double NewtonRaphsonRootFind(QVector2D* Q, QVector2D P, double u);
static QVector2D BezierII(int degree, QVector2D* V, double t);
static double B0(double u);
static double B1(double u);
static double B2(double u);
static double B3(double u);
static QVector2D ComputeLeftTangent(vector<QVector2D>& d,int end);
static QVector2D ComputeRightTangent(vector<QVector2D>& d,int end);
static QVector2D ComputeCenterTangent(vector<QVector2D>& d,int center);
static double* ChordLengthParameterize(vector<QVector2D>& d,int first,int last);



static double ComputeMaxError(vector<QVector2D>& d, int first, int last, QVector2D* bezCurve, double* u, int& splitPoint);
static double V2Dot(QVector2D& a, QVector2D& b);
