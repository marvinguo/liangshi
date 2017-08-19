#pragma once

#include "BanElement.h"

class LIANGSHIEXPORT BanStateElement : public Element
{
	friend class BanBaseWidget;
public:
	BanStateElement();
	BanStateElement(BanElement* banElement);
	virtual ~BanStateElement();
	void SetBanElement(BanElement* element);

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
	BanElement* _banElement;
	string _banElementUuid;
	SPoint _leftTop;
	int _zoomWidth;
	int _zoomHeight;

public:
	MSGPACK_DEFINE(_objUuid, _matrix, _leftTop, _zoomWidth, _zoomHeight, _banElementUuid);
};

