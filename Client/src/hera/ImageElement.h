#pragma once

#include "ImageBaseElement.h"
#include "ImageElementHelper.h"
#include "ImageBuffer.h"

//只用paste的用这个
class LIANGSHIEXPORT ImageElement : public ImageBaseElement
{
	friend class BanBaseWidget;
	friend class PlayerPage;
	friend class LiveWidget;

public:
	ImageElement();
	ImageElement(QPoint leftTop, QPixmap& image);
	virtual ~ImageElement();

public:
	virtual ElementType GetType();
	virtual LokiBuffer GetMsgPackData();
	virtual void UpdateDataFromMsgPack();

public:
	virtual QPixmap GetPixmap();

private:
	void Init(QPoint leftTop, QPixmap& image);

protected:
	ImageBuffer _imageBuffer;
public:
	MSGPACK_DEFINE(_objUuid, _matrix, _leftTop, _width, _height, _imageBuffer);
};

