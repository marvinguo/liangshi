#pragma once

#include "BanElement.h"

class LIANGSHIEXPORT BanElementWithLabel : public BanElement
{
public:
	BanElementWithLabel();
	BanElementWithLabel(BanBaseWidget* viwer);
	virtual ~BanElementWithLabel();

public:
	virtual bool HasLabel(){return true;}
	virtual void SetLabel(QString title);
	virtual QString GetLabel();

public:
//Element virtual
	virtual void SetLineColor(QColor color);
	virtual ElementType GetType();
	virtual bool IsPointIn(QPoint point);
	virtual QRect GetActualRect();
	virtual void DrawOnQtPainter(QPainter* painter);
	virtual LokiBuffer GetMsgPackData();

protected:
	string _label;

public:
	MSGPACK_DEFINE(_objUuid, _matrix, _leftTop, _leftTopInSc, _width, _height, _xZoom, _yZoom, _zoomWidth, _zoomHeight, _data, _label, _lineColor);
};

