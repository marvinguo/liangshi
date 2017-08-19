#pragma once

#include "ImageBaseElement.h"

class LIANGSHIEXPORT ImageStateElement : public Element
{
	friend class BanBaseWidget;
public:
	ImageStateElement();
	ImageStateElement(ImageBaseElement* imageElement);
	virtual ~ImageStateElement();
	void SetImageElement(ImageBaseElement* element);

public:
//Element virtual
	virtual void UpdateSize(QPoint point, int width = -1, int height = -1);
	virtual ElementType GetType();
	virtual LokiBuffer GetMsgPackData();
	virtual void UpdateDataFromMsgPack();
	virtual bool IsPointIn(QPoint point);
	virtual QRect GetActualRect();
	virtual void DrawOnQtPainter(QPainter* painter);
	virtual void SaveInitState();
	virtual void ToInitState();
	virtual void ToCurrentState();

protected:
	ImageBaseElement* _imageElement;
	string _imageElementUuid;
	SPoint _leftTop;
	int _width;
	int _height;

public:
	MSGPACK_DEFINE(_objUuid, _matrix, _leftTop, _width, _height, _imageElementUuid);
};

