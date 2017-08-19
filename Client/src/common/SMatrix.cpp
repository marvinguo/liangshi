#include "SMatrix.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

SMatrix::SMatrix()
{
	QMatrix matrix;
	_m11 = matrix.m11();
	_m12 = matrix.m12();
	_m22 = matrix.m22();
	_m21 = matrix.m21();
	_dx = matrix.dx();
	_dy = matrix.dy();
}

SMatrix::SMatrix(const SMatrix& p)
{
	_m11 = p._m11;
	_m12 = p._m12;
	_m22 = p._m22;
	_m21 = p._m21;
	_dx = p._dx;
	_dy = p._dy;
}

SMatrix::~SMatrix()
{

}

SMatrix& SMatrix::operator= (const QMatrix &another)
{
	_m11 = another.m11();
	_m12 = another.m12();
	_m22 = another.m22();
	_m21 = another.m21();
	_dx = another.dx();
	_dy = another.dy();
	return *this;
}

SMatrix& SMatrix::operator= (const SMatrix &another)
{
	_m11 = another._m11;
	_m12 = another._m12;
	_m22 = another._m22;
	_m21 = another._m21;
	_dx = another._dx;
	_dy = another._dy;
	return *this;
}

QMatrix SMatrix::ToQMatrix()
{
	QMatrix matrix;
	matrix.setMatrix(_m11, _m12, _m21, _m22, _dx, _dy);
	return matrix;
}