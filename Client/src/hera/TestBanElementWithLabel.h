#pragma once

#include "BanElementWithLabel.h"
#include "TestBaseWidget.h"

class LIANGSHIEXPORT TestBanElementWithLabel : public BanElementWithLabel
{
public:
	TestBanElementWithLabel();
	TestBanElementWithLabel(TestBaseWidget* viwer);
	virtual ~TestBanElementWithLabel();

public:
//Element virtual
	virtual ElementType GetType();
	virtual LokiBuffer GetMsgPackData();
	virtual void UpdateDataFromMsgPack();

public:
	MSGPACK_DEFINE(_objUuid, _matrix, _leftTop, _leftTopInSc, _width, _height, _xZoom, _yZoom, _zoomWidth, _zoomHeight, _data, _label, _lineColor);
};

