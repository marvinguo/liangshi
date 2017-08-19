#include "ImageElement.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ImageElement::ImageElement()
{
	QPixmap pixmap;
	Init(QPoint(0, 0), pixmap);
}

ImageElement::ImageElement(QPoint leftTop, QPixmap& image)
{
	Init(leftTop, image);
}

ImageElement::~ImageElement()
{
	if(_helper != NULL)
	{
		_helper->deleteLater();
		_helper->close();
	}
}

QPixmap ImageElement::GetPixmap()
{
	return _imageBuffer.GetImage();
}

void ImageElement::Init(QPoint leftTop, QPixmap& image)
{
	_helper = NULL;
	_leftTop = leftTop;
	_imageBuffer.SetImage(image);
	_width = image.width();
	_height = image.height();
	_cachePixmap = image.copy();
}

ElementType ImageElement::GetType()
{
	return ElementType_Image;
}


LokiBuffer ImageElement::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

void ImageElement::UpdateDataFromMsgPack()
{
	Element::UpdateDataFromMsgPack();
	_cachePixmap = _imageBuffer.GetImage();
}


