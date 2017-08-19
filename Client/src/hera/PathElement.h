#pragma once

#include "Element.h"

class LIANGSHIEXPORT PathElement : public Element
{
	struct State
	{
		int width;
		int style;
		QColor color;
	};
protected:
	PathElement();
public:
	virtual ~PathElement();

//element virtual
public:
	virtual bool IsPointIn(QPoint point);
	virtual QRect GetActualRect();
	virtual void Highlight(QWidget* parent, bool high = true);
	virtual void DrawOnQtPainter(QPainter* painter);
	virtual void SaveInitState();
	virtual void ToInitState();
	virtual void ToCurrentState();

protected:
	QPainterPath _path;
	State _initState;
	State _currentState;
};

