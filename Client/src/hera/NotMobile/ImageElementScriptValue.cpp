#include "ImageElementScriptValue.h"
#include "CompressedImageElement.h"
#include "ImageElement.h"
#include "ImageStateElement.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ImageElementScriptValue::ImageElementScriptValue(QScriptEngine *engine, BanScriptWidget* widget, QString path):
QObject(engine),
_engine(engine),
_widget(widget),
_element(NULL)
{
	QPixmap pixmap;
	pixmap.load(path);
	
	//这个element,也需要加入到BanBaseWidet中吧，但是不能显示....
	QPoint point(0, 0);
	if(pixmap.hasAlpha())
	{
		_element = new ImageElement(point, pixmap);
	}
	else
	{
		_element = new CompressedImageElement(point, pixmap);
	}

	_element->SetCanPaint(false);
	_element->SetCanPreview(false);
	_widget->AddElement(_element, true, false);
}

ImageElementScriptValue::~ImageElementScriptValue()
{
}

void ImageElementScriptValue::InitScriptValue(QScriptValue& value)
{
	QScriptValue getWidth = _engine->newFunction(ImageElementScriptValue::getWidthFunction, this);
	value.setProperty(tr("宽"), getWidth);
	value.setProperty(tr("width"), getWidth);
	QScriptValue getHeight = _engine->newFunction(ImageElementScriptValue::getHeightFunction, this);
	value.setProperty(tr("高"), getHeight);
	value.setProperty(tr("height"), getHeight);
	QScriptValue draw = _engine->newFunction(ImageElementScriptValue::drawFunction, this);
	value.setProperty(tr("画"), draw);
}

QScriptValue ImageElementScriptValue::ScriptValue()
{
	QScriptValue value = _engine->newQObject(this);
	InitScriptValue(value);
	return value;
}

//这里有问题
QScriptValue ImageElementScriptValue::draw(int x, int y)
{
	ImageStateElement* element = new ImageStateElement(_element);
	QPoint point(x, y);
	element->UpdateSize(point);
	_widget->AddElement(element, true, false);
	_widget->DeHighlightElement();
	return QScriptValue();
}

int ImageElementScriptValue::GetWidth()
{
	QRect rect = _element->GetActualRect();
	return rect.width();
}

int ImageElementScriptValue::GetHeight()
{
	QRect rect = _element->GetActualRect();
	return rect.height();
}

QScriptValue ImageElementScriptValue::getWidthFunction(QScriptContext* context, QScriptEngine* engine, void* param)
{
	ImageElementScriptValue* value = (ImageElementScriptValue*)(param);
	return value->GetWidth();
}

QScriptValue ImageElementScriptValue::getHeightFunction(QScriptContext* context, QScriptEngine* engine, void* param)
{
	ImageElementScriptValue* value = (ImageElementScriptValue*)(param);
	return value->GetHeight();
}

QScriptValue ImageElementScriptValue::drawFunction(QScriptContext* context, QScriptEngine* engine, void* param)
{
	ImageElementScriptValue* value = (ImageElementScriptValue*)(param);
	if(context->argumentCount() == 2)
	{
		QScriptValue parameter1 = context->argument(0);
		QScriptValue parameter2 = context->argument(1);

		if (parameter1.isNumber() && parameter2.isNumber())
		{
			int x1 = parameter1.toInt32();
			int y1 = parameter2.toInt32();
			return value->draw(x1, y1);
		}
	}
	return QScriptValue();
}

