#include "TextElementScriptValue.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

TextElementScriptValue::TextElementScriptValue(QScriptEngine *engine, BanScriptWidget* widget, QString text, int size):
QObject(engine),
_engine(engine),
_widget(widget),
_element(NULL)
{
	QPoint point(0, 0);
	_element = new TextElement(point, text);
	_element->_fontSize = size;
}

TextElementScriptValue::~TextElementScriptValue()
{
}

void TextElementScriptValue::InitScriptValue(QScriptValue& value)
{
	QScriptValue getWidth = _engine->newFunction(TextElementScriptValue::getWidthFunction, this);
	value.setProperty(tr("宽"), getWidth);
	value.setProperty(tr("width"), getWidth);
	QScriptValue getHeight = _engine->newFunction(TextElementScriptValue::getHeightFunction, this);
	value.setProperty(tr("高"), getHeight);
	value.setProperty(tr("height"), getHeight);
	QScriptValue draw = _engine->newFunction(TextElementScriptValue::drawFunction, this);
	value.setProperty(tr("画"), draw);
}

QScriptValue TextElementScriptValue::ScriptValue()
{
	QScriptValue value = _engine->newQObject(this);
	InitScriptValue(value);
	return value;
}

QScriptValue TextElementScriptValue::draw(int x, int y)
{
	QPoint point(x, y);
	_element->UpdateSize(point);
	_widget->AddElement(_element, true, false);
	_widget->DeHighlightElement();
	return QScriptValue();
}

int TextElementScriptValue::GetWidth()
{
	QRect rect = _element->GetActualRect();
	return rect.width();
}

int TextElementScriptValue::GetHeight()
{
	QRect rect = _element->GetActualRect();
	return rect.height();
}

QScriptValue TextElementScriptValue::getWidthFunction(QScriptContext* context, QScriptEngine* engine, void* param)
{
	TextElementScriptValue* value = (TextElementScriptValue*)(param);
	return value->GetWidth();
}

QScriptValue TextElementScriptValue::getHeightFunction(QScriptContext* context, QScriptEngine* engine, void* param)
{
	TextElementScriptValue* value = (TextElementScriptValue*)(param);
	return value->GetHeight();
}

QScriptValue TextElementScriptValue::drawFunction(QScriptContext* context, QScriptEngine* engine, void* param)
{
	TextElementScriptValue* value = (TextElementScriptValue*)(param);
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

