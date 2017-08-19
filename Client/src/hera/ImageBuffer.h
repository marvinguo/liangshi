#pragma once

#include "../common/Qt.h"
#include "../common/library.h"
#include "../../../SharedCode/LokiBuffer.h"

//MsgPack image, jpeg inside
class LIANGSHIEXPORT ImageBuffer
{
public:
	ImageBuffer(){}
	ImageBuffer(ImageBuffer& p){}
	~ImageBuffer(){}

	virtual void SetImage(QPixmap image);
	virtual QPixmap GetImage();

protected:
	LokiBuffer _imageData;

public:
	MSGPACK_DEFINE(_imageData);
};

