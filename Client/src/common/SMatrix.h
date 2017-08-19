#pragma once

#include "Qt.h"
#include "library.h"

class SMatrix
{
public:
	SMatrix();
	SMatrix(const SMatrix& p);
	~SMatrix();

	SMatrix& operator= (const QMatrix &another);
	SMatrix& operator= (const SMatrix &another);

	QMatrix ToQMatrix();

public:
	MSGPACK_DEFINE(_m11, _m12, _m22, _m21, _dx, _dy)

private:
	double _m11;
	double _m12;
	double _m22;
	double _m21;
	double _dx;
	double _dy;
};

