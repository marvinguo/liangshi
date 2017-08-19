#include "FitCurves.h"

bool isDoubleZero(double& d)
{
	//if (denominator == 0.0f)
	if(d > -0.01 && d < 0.01)
		return true;
	return false;
}

const int MAXPOINTS = 10000;
vector<QVector2D> FitCurve(vector<QVector2D>& d, double error)
{
	QVector2D tHat1, tHat2;	/*  Unit tangent vectors at endpoints */

	tHat1 = ComputeLeftTangent(d, 0);
	tHat2 = ComputeRightTangent(d, d.size() - 1);
	vector<QVector2D> result;
	FitCubic(d, 0, d.size() - 1, tHat1, tHat2, error,result);
	return result;
}

static void FitCubic(vector<QVector2D>& d, int first, int last, QVector2D tHat1, QVector2D tHat2, double error,vector<QVector2D>& result)
{
	QVector2D* bezCurve; /*Control points of fitted Bezier curve*/
	double* u;		/*  Parameter values for point  */
	double* uPrime;	/*  Improved parameter values */
	double maxError;	/*  Maximum fitting error	 */
	int splitPoint;	/*  Point to split point set at	 */
	int nPts;		/*  Number of points in subset  */
	double iterationError; /*Error below which you try iterating  */
	int maxIterations = 4; /*  Max times to try iterating  */
	QVector2D tHatCenter;   	/* Unit tangent vector at splitPoint */
	int i;

	iterationError = error * error;
	nPts = last - first + 1;

	/*  Use heuristic if region only has two points in it */
	if(nPts == 2)
	{
		double dist = (d[first]-d[last]).length() / 3.0;

		bezCurve = new QVector2D[4];
		bezCurve[0] = d[first];
		bezCurve[3] = d[last];
		QVector2D p1 = tHat1 * dist;
		bezCurve[1] = p1 + bezCurve[0];
		QVector2D p2 = tHat2 * dist;
		bezCurve[2] = p2 + bezCurve[3];

		result.push_back(bezCurve[1]);
		result.push_back(bezCurve[2]);
		result.push_back(bezCurve[3]);
		delete[] bezCurve;
		return;
	}

	/*  Parameterize points, and attempt to fit curve */
	u = ChordLengthParameterize(d, first, last);
	bezCurve = GenerateBezier(d, first, last, u, tHat1, tHat2);

	/*  Find max deviation of points to fitted curve */
	maxError = ComputeMaxError(d, first, last, bezCurve, u, splitPoint);
	if(maxError < error)
	{
		result.push_back(bezCurve[1]);
		result.push_back(bezCurve[2]);
		result.push_back(bezCurve[3]);
		delete[] u;
		delete[] bezCurve;
		return;
	}

	/*  If error not too large, try some reparameterization  */
	/*  and iteration */
	if(maxError < iterationError)
	{
		for(i = 0; i < maxIterations; i++)
		{
			uPrime = Reparameterize(d, first, last, u, bezCurve);
			delete[] bezCurve;
			bezCurve = GenerateBezier(d, first, last, uPrime, tHat1, tHat2);
			maxError = ComputeMaxError(d, first, last, bezCurve, uPrime, splitPoint);
			if(maxError < error)
			{
				result.push_back(bezCurve[1]);
				result.push_back(bezCurve[2]);
				result.push_back(bezCurve[3]);
				delete[] bezCurve;
				delete[] u;
				delete[] uPrime;
				return;
			}
			delete[] u;
			u = uPrime;
		}
	}

	delete[] bezCurve;
	delete[] u;
	/* Fitting failed -- split at max error point and fit recursively */
	tHatCenter = ComputeCenterTangent(d, splitPoint);
	FitCubic(d, first, splitPoint, tHat1, tHatCenter, error,result);
	tHatCenter *= -1;
	FitCubic(d, splitPoint, last, tHatCenter, tHat2, error,result);
}

static QVector2D* GenerateBezier(vector<QVector2D>& d, int first, int last, double* uPrime, QVector2D& tHat1, QVector2D& tHat2)
{
	int 	i;
	QVector2D A[MAXPOINTS* 2];/* Precomputed rhs for eqn	*/

	int 	nPts;			/* Number of pts in sub-curve */
	double 	C[2 * 2];			/* Matrix C		*/
	double 	X[2];			/* Matrix X			*/
	double 	det_C0_C1,		/* Determinants of matrices	*/
		det_C0_X,
		det_X_C1;
	double 	alpha_l,		/* Alpha values, left and right	*/
		alpha_r;
	QVector2D 	tmp;			/* Utility variable		*/
	QVector2D* bezCurve = new QVector2D[4];	/* RETURN bezier curve ctl pts	*/
	nPts = last - first + 1;

	/* Compute the A's	*/
	for (i = 0; i < nPts; i++)
	{
		QVector2D		v1, v2;
		v1 = tHat1;
		v2 = tHat2;
		v1 = v1 * (B1(uPrime[i]));
		v2 = v2 * (B2(uPrime[i]));
		A[i,0] = v1;
		A[i,1] = v2;
	}

	/* Create the C and X matrices	*/
	C[0,0] = 0.0;
	C[0,1] = 0.0;
	C[1,0] = 0.0;
	C[1,1] = 0.0;
	X[0]    = 0.0;
	X[1]    = 0.0;

	for (i = 0; i < nPts; i++)
	{
		C[0,0] +=  V2Dot(A[i,0], A[i,0]);
		C[0,1] += V2Dot(A[i,0], A[i,1]);
		/*					C[1][0] += V2Dot(&A[i][0], &A[i][1]);*/	
		C[1,0] = C[0,1];
		C[1,1] += V2Dot(A[i,1], A[i,1]);
		QVector2D add1 = d[first] * B0(uPrime[i]);
		QVector2D add2 = d[first] * B1(uPrime[i]);
		QVector2D add3 = d[last] * B2(uPrime[i]);
		QVector2D add4 = d[last] * B3(uPrime[i]);
		tmp = d[first + i] - (add1 + add2 + add3 + add4);


		X[0] += V2Dot(A[i,0], tmp);
		X[1] += V2Dot(A[i,1], tmp);
	}

	/* Compute the determinants of C and X	*/
	det_C0_C1 = C[0,0] * C[1,1] - C[1,0] * C[0,1];
	det_C0_X  = C[0,0] * X[1]    - C[1,0] * X[0];
	det_X_C1  = X[0]    * C[1,1] - X[1]    * C[0,1];

	/* Finally, derive alpha values	*/
	//alpha_l = (det_C0_C1 == 0) ? 0.0 : det_X_C1 / det_C0_C1;
	//alpha_r = (det_C0_C1 == 0) ? 0.0 : det_C0_X / det_C0_C1;
	
	alpha_l = isDoubleZero(det_C0_C1) ? 0.0 : det_X_C1 / det_C0_C1;
	alpha_r = isDoubleZero (det_C0_C1) ? 0.0 : det_C0_X / det_C0_C1;

	/* If alpha negative, use the Wu/Barsky heuristic (see text) */
	/* (if alpha is 0, you get coincident control points that lead to
	* divide by zero in any subsequent NewtonRaphsonRootFind() call. */
	double segLength = (d[first] - d[last]).length();
	double epsilon = 1.0e-6 * segLength;
	if (alpha_l < epsilon || alpha_r < epsilon)
	{
		/* fall back on standard (probably inaccurate) formula, and subdivide further if needed. */
		double dist = segLength / 3.0;
		bezCurve[0] = d[first];
		bezCurve[3] = d[last];
		QVector2D p1 = tHat1 * dist;
		bezCurve[1] = p1 + bezCurve[0];
		QVector2D p2 = tHat2 * dist;
		bezCurve[2] = p2 + bezCurve[3];
		return (bezCurve);
	}

	/*  First and last control points of the Bezier curve are */
	/*  positioned exactly at the first and last data points */
	/*  Control points 1 and 2 are positioned an alpha distance out */
	/*  on the tangent vectors, left and right, respectively */
	bezCurve[0] = d[first];
	bezCurve[3] = d[last];
	QVector2D p1= tHat1 * alpha_l;
	bezCurve[1] = p1 + bezCurve[0];
	QVector2D p2 = tHat2 * alpha_r;
	bezCurve[2] = p2 + bezCurve[3];
	return (bezCurve);
}

static double* Reparameterize(vector<QVector2D>& d,int first,int last,double* u, QVector2D* bezCurve)
{
	int 	nPts = last-first+1;	
	int 	i;
	double*	uPrime = new double[nPts];		/*  New parameter values	*/

	for (i = first; i <= last; i++) {
		uPrime[i-first] = NewtonRaphsonRootFind(bezCurve, d[i], u[i-first]);
	}
	return uPrime;
}

static double NewtonRaphsonRootFind(QVector2D* Q, QVector2D P, double u)
{
	double 		numerator, denominator;
	QVector2D Q1[3], Q2[2];	/*  Q' and Q''			*/
	QVector2D		Q_u, Q1_u, Q2_u; /*u evaluated at Q, Q', & Q''	*/
	double 		uPrime;		/*  Improved u			*/
	int 		i;

	/* Compute Q(u)	*/
	Q_u = BezierII(3, Q, u);

	/* Generate control vertices for Q'	*/
	for (i = 0; i <= 2; i++)
	{
		Q1[i].setX((Q[i+1].x() - Q[i].x()) * 3.0);
		Q1[i].setY((Q[i+1].y() - Q[i].y()) * 3.0);
	}

	/* Generate control vertices for Q'' */
	for (i = 0; i <= 1; i++) 
	{
		Q2[i].setX((Q1[i+1].x() - Q1[i].x()) * 2.0);
		Q2[i].setY((Q1[i+1].y() - Q1[i].y()) * 2.0);
	}

	/* Compute Q'(u) and Q''(u)	*/
	Q1_u = BezierII(2, Q1, u);
	Q2_u = BezierII(1, Q2, u);

	/* Compute f(u)/f'(u) */
	numerator = (Q_u.x() - P.x()) * (Q1_u.x()) + (Q_u.y() - P.y()) * (Q1_u.y());
	denominator = (Q1_u.x()) * (Q1_u.x()) + (Q1_u.y()) * (Q1_u.y()) +
		(Q_u.x() - P.x()) * (Q2_u.x()) + (Q_u.y() - P.y()) * (Q2_u.y());

	//if (denominator == 0.0f)
	if(isDoubleZero(denominator))
		return u;

	/* u = u - f(u)/f'(u) */
	uPrime = u - (numerator/denominator);
	return (uPrime);
}

static QVector2D BezierII(int degree, QVector2D* V, double t)
{
	int 	i, j;		
	QVector2D 	Q;	        /* Point on curve at parameter t	*/
	QVector2D* 	Vtemp;		/* Local copy of control points		*/

	/* Copy array	*/
	Vtemp = new QVector2D[degree+1];
	for (i = 0; i <= degree; i++) {
		Vtemp[i] = V[i];
	}

	/* Triangle computation	*/
	for (i = 1; i <= degree; i++) {	
		for (j = 0; j <= degree-i; j++) 
		{
			Vtemp[j].setX((1.0 - t) * Vtemp[j].x() + t * Vtemp[j+1].x());
			Vtemp[j].setY((1.0 - t) * Vtemp[j].y() + t * Vtemp[j+1].y());
		}
	}

	Q = Vtemp[0];
	delete[] Vtemp;
	return Q;
}

static double B0(double u)
{
	double tmp = 1.0 - u;
	return (tmp * tmp * tmp);
}


static double B1(double u)
{
	double tmp = 1.0 - u;
	return (3 * u * (tmp * tmp));
}

static double B2(double u)
{
	double tmp = 1.0 - u;
	return (3 * u * u * tmp);
}

static double B3(double u)
{
	return (u * u * u);
}

static QVector2D ComputeLeftTangent(vector<QVector2D>& d,int end)
{
	QVector2D	tHat1;
	tHat1 = d[end+1]- d[end];
	tHat1.normalize();
	return tHat1;
}

static QVector2D ComputeRightTangent(vector<QVector2D>& d,int end)
{
	QVector2D	tHat2;
	tHat2 = d[end-1] - d[end];
	tHat2.normalize();
	return tHat2;
}

static QVector2D ComputeCenterTangent(vector<QVector2D>& d,int center)
{
	QVector2D	V1, V2, tHatCenter;

	V1 = d[center-1] - d[center];
	V2 = d[center] - d[center+1];
	tHatCenter.setX((V1.x() + V2.x())/2.0);
	tHatCenter.setY((V1.y() + V2.y())/2.0);
	tHatCenter.normalize();
	return tHatCenter;
}

static double* ChordLengthParameterize(vector<QVector2D>& d,int first,int last)
{
	int		i;	
	double*	u = new double[last-first+1];			/*  Parameterization		*/

	u[0] = 0.0;
	for (i = first+1; i <= last; i++) {
		u[i-first] = u[i-first-1] + (d[i-1] - d[i]).length();
	}

	for (i = first + 1; i <= last; i++) {
		u[i-first] = u[i-first] / u[last-first];
	}

	return u;
}

static double ComputeMaxError(vector<QVector2D>& d, int first, int last, QVector2D* bezCurve, double* u, int& splitPoint)
{
	int i;
	double	maxDist;		/*  Maximum error		*/
	double	dist;		/*  Current error		*/
	QVector2D	P;			/*  Point on curve		*/
	QVector2D	v;			/*  QVector2D from point to curve	*/

	splitPoint = (last - first + 1)/2;
	maxDist = 0.0;
	for (i = first + 1; i < last; i++) {
		P = BezierII(3, bezCurve, u[i-first]);
		v = P - d[i];
		dist = v.length() * v.length();
		if (dist >= maxDist) {
			maxDist = dist;
			splitPoint = i;
		}
	}
	return maxDist;
}

static double V2Dot(QVector2D& a, QVector2D& b) 
{
	return((a.x()*b.x())+(a.y()*b.y()));
}