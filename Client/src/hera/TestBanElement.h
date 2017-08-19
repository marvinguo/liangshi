#pragma once

#include "BanElement.h"
#include "TestBaseWidget.h"

class LIANGSHIEXPORT TestBanElement : public BanElement
{
public:
	TestBanElement();
	TestBanElement(TestBaseWidget* viwer);
	virtual ~TestBanElement();

public:
//Element virtual
	virtual ElementType GetType();
	virtual LokiBuffer GetMsgPackData();
	virtual void UpdateDataFromMsgPack();

public:
	MSGPACK_DEFINE(_objUuid, _matrix, _leftTop, _leftTopInSc, _width, _height, _xZoom, _yZoom, _zoomWidth, _zoomHeight, _data);
};

