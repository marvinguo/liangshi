#pragma once

#include "Qt.h"
#include "library.h"


//关于点
//QPoint里是int
//QVector2D里面是float
//保存在文件里，一律用int型，板书里或是原始文件里，不做缩放

class LIANGSHIEXPORT SPoint
{
public:
	SPoint();
	SPoint(int x, int y);
	SPoint(const SPoint& p);
	explicit SPoint(const QPoint& p);
	explicit SPoint(const QVector2D& p);

	~SPoint();

	SPoint& operator= (const SPoint &another);
	SPoint& operator= (const QPoint &another);

	QPoint ToQPoint();

public:
	int _x;
	int _y;

public:
	MSGPACK_DEFINE(_x, _y);
};

