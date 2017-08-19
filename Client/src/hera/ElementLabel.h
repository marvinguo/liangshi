#pragma once

#include "AnimationAction.h"
#include "../common/Qt.h"

class LIANGSHIEXPORT ElementLabel : public QLabel
{
	Q_OBJECT
public:
	ElementLabel(QWidget *parent = 0);
	~ElementLabel();

private:
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	virtual void wheelEvent(QWheelEvent *e);

private:
	QPoint _clickPoint;
	bool _isMoving;
};

