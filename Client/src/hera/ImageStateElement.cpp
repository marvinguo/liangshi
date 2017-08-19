#include "ImageStateElement.h"
#include "BanFile.h"
#include "../common/Util.h"
#include "BanBaseWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ImageStateElement::ImageStateElement():
Element(),
_imageElement(NULL)
{
}

ImageStateElement::ImageStateElement(ImageBaseElement* imageElement):
Element()
{
	SetImageElement(imageElement);
}

ImageStateElement::~ImageStateElement()
{
}

void ImageStateElement::SetImageElement(ImageBaseElement* element)
{
	_imageElement = element;
	_imageElementUuid = element->GetObjectUuid();
	_width = element->GetWidth();
	_height = element->GetHeight();
}

//image state也可以做动画？好像也可以，不知道
void ImageStateElement::SaveInitState()
{
}

void ImageStateElement::ToInitState()
{
}

void ImageStateElement::ToCurrentState()
{
}

ElementType ImageStateElement::GetType()
{
	return ElementType_ImageState;
}

void ImageStateElement::UpdateSize(QPoint point, int width, int height)
{
	_leftTop = point;
	if(width != -1)
		_width = width;
	if(height != -1)
		_height = height;
}

LokiBuffer ImageStateElement::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

bool ImageStateElement::IsPointIn(QPoint point)
{
	ImageBaseElement::State oldState = _imageElement->GetState();
	ImageBaseElement::State state;
	state.leftTop = _leftTop;
	state.width = _width;
	state.height = _height;
	state.matrix = _matrix;
	_imageElement->SetState(state);
	bool result = _imageElement->IsPointIn(point);
	_imageElement->SetState(oldState);
	return result;
}

QRect ImageStateElement::GetActualRect()
{
	ImageBaseElement::State oldState = _imageElement->GetState();
	ImageBaseElement::State state;
	state.leftTop = _leftTop;
	state.width = _width;
	state.height = _height;
	state.matrix = _matrix;
	_imageElement->SetState(state);
	QRect rect = _imageElement->GetActualRect();
	_imageElement->SetState(oldState);
	return rect;
}

void ImageStateElement::DrawOnQtPainter(QPainter* painter)
{
	//保存以前的state
	//换上现在的state,matrix也需要
	//draw
	//换回来
	if(_imageElement != NULL)
	{
		ImageBaseElement::State oldState = _imageElement->GetState();
		ImageBaseElement::State state;
		state.leftTop = _leftTop;
		state.width = _width;
		state.height = _height;
		state.matrix = _matrix;
		_imageElement->SetState(state);
		_imageElement->DrawOnQtPainter(painter);
		_imageElement->SetState(oldState);
	}
}

void ImageStateElement::UpdateDataFromMsgPack()
{
	Element::UpdateDataFromMsgPack();
	//需要找到element的指针
}