#pragma once

#include "ImageBuffer.h"

//MsgPack image, jpeg inside
class LIANGSHIEXPORT CompressedImageBuffer : public ImageBuffer
{
public:
	CompressedImageBuffer(){}
	CompressedImageBuffer(CompressedImageBuffer& p){}
	~CompressedImageBuffer(){}

	virtual void SetImage(QPixmap image);
	virtual QPixmap GetImage();

public:
	MSGPACK_DEFINE(_imageData);
};

