#pragma once

#include "ImageBaseElement.h"
#include "ImageElementHelper.h"
#include "CompressedImageBuffer.h"

//screen, camera都用这个
class LIANGSHIEXPORT CompressedImageElement : public ImageBaseElement
{
public:
	CompressedImageElement();
	CompressedImageElement(QPoint leftTop, QPixmap& image);
	virtual ~CompressedImageElement();

public:
	virtual ElementType GetType();
	virtual LokiBuffer GetMsgPackData();
	virtual void UpdateDataFromMsgPack();

public:
	virtual QPixmap GetPixmap();

private:
	void Init(QPoint leftTop, QPixmap& image);

protected:
	CompressedImageBuffer _imageBuffer;

public:
	MSGPACK_DEFINE(_objUuid, _matrix, _leftTop, _width, _height, _imageBuffer);
};

