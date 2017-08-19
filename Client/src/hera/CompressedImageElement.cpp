#include "CompressedImageElement.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

CompressedImageElement::CompressedImageElement()
{
	QPixmap pixmap;
	Init(QPoint(0, 0), pixmap);
}

CompressedImageElement::CompressedImageElement(QPoint leftTop, QPixmap& image)
{
	Init(leftTop, image);
}

CompressedImageElement::~CompressedImageElement()
{
	if(_helper != NULL)
	{
		_helper->deleteLater();
		_helper->close();
	}
}

QPixmap CompressedImageElement::GetPixmap()
{
	return _imageBuffer.GetImage();
}

void CompressedImageElement::Init(QPoint leftTop, QPixmap& image)
{
	_helper = NULL;
	_leftTop = leftTop;
	_imageBuffer.SetImage(image);
	_width = image.width();
	_height = image.height();
	_cachePixmap = image.copy();
}

ElementType CompressedImageElement::GetType()
{
	return ElementType_CompressedImage;
}

LokiBuffer CompressedImageElement::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

void CompressedImageElement::UpdateDataFromMsgPack()
{
	Element::UpdateDataFromMsgPack();
	_cachePixmap = _imageBuffer.GetImage();
}


