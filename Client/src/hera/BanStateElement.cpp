#include "BanStateElement.h"
#include "BanFile.h"
#include "../common/Util.h"
#include "BanBaseWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BanStateElement::BanStateElement():
Element(),
_banElement(NULL)
{
}

BanStateElement::BanStateElement(BanElement* banElement):
Element()
{
	SetBanElement(banElement);
}

BanStateElement::~BanStateElement()
{
}

void BanStateElement::SetBanElement(BanElement* element)
{
	_banElement = element;
	_banElementUuid = element->GetObjectUuid();
	_zoomWidth = element->_zoomWidth;
	_zoomHeight =  element->_zoomHeight;
}

void BanStateElement::SaveInitState()
{
}

void BanStateElement::ToInitState()
{
}

void BanStateElement::ToCurrentState()
{
}

ElementType BanStateElement::GetType()
{
	return ElementType_BanState;
}

void BanStateElement::UpdateSize(QPoint point, int width, int height)
{
	_leftTop = point;
	if(width != -1)
		_zoomWidth = width;
	if(height != -1)
		_zoomHeight = height;
}

LokiBuffer BanStateElement::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

bool BanStateElement::IsPointIn(QPoint point)
{
	BanElement::State oldState = _banElement->GetState();
	BanElement::State state;
	state.leftTop = _leftTop;
	state.zoomWidth = _zoomWidth;
	state.zoomHeight = _zoomHeight;
	state.matrix = _matrix;
	_banElement->SetState(state);
	bool result = _banElement->IsPointIn(point);
	_banElement->SetState(oldState);
	return result;
}

QRect BanStateElement::GetActualRect()
{
	BanElement::State oldState = _banElement->GetState();
	BanElement::State state;
	state.leftTop = _leftTop;
	state.zoomWidth = _zoomWidth;
	state.zoomHeight = _zoomHeight;
	state.matrix = _matrix;
	_banElement->SetState(state);
	QRect rect = _banElement->GetActualRect();
	_banElement->SetState(oldState);
	return rect;
}

void BanStateElement::DrawOnQtPainter(QPainter* painter)
{
	if(_banElement != NULL)
	{
		BanElement::State oldState = _banElement->GetState();
		BanElement::State state;
		state.leftTop = _leftTop;
		state.zoomWidth = _zoomWidth;
		state.zoomHeight = _zoomHeight;
		state.matrix = _matrix;
		_banElement->SetState(state);
		_banElement->DrawOnQtPainter(painter);
		_banElement->SetState(oldState);
	}
}

void BanStateElement::UpdateDataFromMsgPack()
{
	Element::UpdateDataFromMsgPack();
}