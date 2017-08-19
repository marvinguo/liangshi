#include "../common/library.h"
#include "Element.h"
#include "DrawElement.h"
#include "LineElement.h"
#include "RectElement.h"
#include "EllipseElement.h"
#include "ImageElement.h"
#include "CompressedImageElement.h"
#include "TextElement.h"
#include "HtmlTextElement.h"
#include "BanElement.h"
#include "BanElementWithLabel.h"
#include "TestBanElement.h"
#include "TestBanElementWithLabel.h"
#include "OrigialDrawElement.h"
#include "BanStateElement.h"
#include "ImageStateElement.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

Element* Element::CreateFromType(ElementType type, LokiBuffer& buffer)
{
	msgpack::unpacked msg;
	msgpack::unpack(&msg, buffer.Data(), buffer.Size());

	Element *element = NULL;
	try
	{
		if(type == ElementType_Draw)
		{
			DrawElement* e = new DrawElement();
			msg.get().convert(e);
			element = e;
		}
		else if(type == ElementType_Line)
		{
			LineElement* e = new LineElement();
			msg.get().convert(e);
			element = e;
		}
		else if(type == ElementType_Rect)
		{
			RectElement* e = new RectElement();
			msg.get().convert(e);
			element = e;
		}
		else if(type == ElementType_Ellipse)
		{
			EllipseElement* e = new EllipseElement();
			msg.get().convert(e);
			element = e;
		}
		else if(type == ElementType_Image)
		{
			ImageElement* e = new ImageElement();
			msg.get().convert(e);
			element = e;
		}
		else if(type == ElementType_Text)
		{
			TextElement* e = new TextElement();
			msg.get().convert(e);
			element = e;
		}
		else if(type == ElementType_Ban)
		{
			BanElement* e = new BanElement();
			msg.get().convert(e);
			element = e;
		}
		else if(type == ElementType_BanWithLabel)
		{
			BanElementWithLabel* e = new BanElementWithLabel();
			msg.get().convert(e);
			element = e;
		}
		else if(type == ElementType_TestBan)
		{
			TestBanElement* e = new TestBanElement();
			msg.get().convert(e);
			element = e;
		}
		else if(type == ElementType_TestBanWithLabel)
		{
			TestBanElementWithLabel* e = new TestBanElementWithLabel();
			msg.get().convert(e);
			element = e;
		}
		else if(type == ElementType_CompressedImage)
		{
			CompressedImageElement* e = new CompressedImageElement();
			msg.get().convert(e);
			element = e;
		}
		else if(type == ElementType_HtmlText)
		{
			HtmlTextElement* e = new HtmlTextElement();
			msg.get().convert(e);
			element = e;
		}
		else if(type == ElementType_OriginalDraw)
		{
			OrigialDrawElement* e = new OrigialDrawElement();
			msg.get().convert(e);
			element = e;
		}
		else if(type == ElementType_BanState)
		{
			BanStateElement* e = new BanStateElement();
			msg.get().convert(e);
			element = e;
		}
		else if(type == ElementType_ImageState)
		{
			ImageStateElement* e = new ImageStateElement();
			msg.get().convert(e);
			element = e;
		}
	}
	catch(...)
	{
		element = NULL;
	}

	if(element != NULL)
		element->UpdateDataFromMsgPack();
	return element;
}

Element* Element::CreateFromCopyableData(LokiBuffer& buffer)
{
	Element* element = NULL;

	msgpack::unpacker pac;
	pac.reserve_buffer(buffer.Size());
	memcpy(pac.buffer(), buffer.Data(), buffer.Size());
	pac.buffer_consumed(buffer.Size());

	int type = 0;

	msgpack::unpacked result;
	while(pac.next(&result))
	{
		msgpack::object object = result.get();

		if(object.type == msgpack::type::POSITIVE_INTEGER)
		{
			object.convert(&type);
		}
		if(object.type == msgpack::type::RAW)
		{
			if(type != 0)
			{
				LokiBuffer rawLokiBuffer(const_cast<char*>(object.via.raw.ptr), object.via.raw.size);
				element = Element::CreateFromType((ElementType)type, rawLokiBuffer);
			}
		}
	}
	return element;
}

Element::Element()
{
	_objUuid = QUuid::createUuid().toString().toStdString();
	_highlight = false;
	_canDelete = true;
	_canPaint = true;
	_canPreview = true;
	Reset(true);
}

Element::~Element()
{

}

void Element::Reset(bool hasMatrix)
{
	//这里需要一下，主要是ScElement还有问题
	//应该直接改变数据，这个行为会做为动态
	QColor color(Qt::white);
	_lineColor = color.rgba();
	_lineQColor = color;
	_lineStyle = Qt::SolidLine;
	_lineWidth = 2;
	if(hasMatrix)
	{
		SetMatrix(QMatrix());
	}
}

string Element::GetObjectUuid()
{
	return _objUuid;
}

void Element::SetLineWidth(int width)
{
	_lineWidth = width;
}

void Element::SetLineStyle(int style)
{
	_lineStyle = style;
}

void Element::SetMatrix(QMatrix matrix)
{
	_matrix = matrix;
}

void Element::SetLineColor(QColor color)
{
	_lineColor = color.rgba();
	_lineQColor = color;
}

void Element::UpdateSize(QPoint point, int width, int height)
{
}

int Element::GetLineWidth()
{
	return _lineWidth;
}

SMatrix Element::GetMatrix()
{
	return _matrix;
}

QColor Element::GetLineColor()
{
	return _lineQColor;
}

bool Element::IsHightlighting()
{
	return _highlight;
}

void Element::SetCanDelete(bool can)
{
	_canDelete = can;
}

bool Element::CanDelete()
{
	return _canDelete;
}

void Element::SetCanPaint(bool can)
{
	_canPaint = can;
}

bool Element::CanPaint()
{
	return _canPaint;
}

void Element::SetCanPreview(bool can)
{
	_canPreview = can;
}

bool Element::CanPreview()
{
	return _canPreview;
}

//这个地方忘了一件事情，就是，如果highlight,就不应该DrawOnQtPainter
void Element::Highlight(QWidget* parent, bool high)
{
	_highlight = high;
}

void Element::UpdateDataFromMsgPack()
{
	_lineQColor = QColor::fromRgba(_lineColor);
	//这里是构造完成，需要saveInitState
	SaveInitState();
}

LokiBuffer Element::GetCopyableData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	LokiBuffer elementData = GetMsgPackData();
	packer.pack((int)GetType());
	packer.pack_raw(elementData.Size());
	packer.pack_raw_body(elementData.Data(), elementData.Size());

	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}
