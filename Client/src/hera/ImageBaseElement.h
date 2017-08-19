#pragma once

#include "Element.h"
#include "ImageElementHelper.h"
#include "ImageBuffer.h"

//只用paste的用这个
class LIANGSHIEXPORT ImageBaseElement : public Element
{
public:
	struct State
	{
		SPoint leftTop;
		int width;
		int height;
		SMatrix matrix;
	};
public:
	ImageBaseElement();
	virtual ~ImageBaseElement();

	virtual QPixmap GetPixmap() = 0;
	virtual void UpdateSize(QPoint point, int width = -1, int height = -1);
	virtual bool IsPointIn(QPoint point);
	virtual QRect GetActualRect();
	virtual void Highlight(QWidget* parent, bool high = true);
	virtual void DrawOnQtPainter(QPainter* painter);
	virtual void SaveInitState();
	virtual void ToInitState();
	virtual void ToCurrentState();
	State GetState();
	void SetState(State state);
	QPoint GetLeftTop();
	int GetWidth();
	int GetHeight();

protected:
	ImageElementHelper* _helper;
	QPixmap _cachePixmap;

	//save part
	SPoint _leftTop;
	int _width;
	int _height;


	State _initState;
	State _currentState;
};

