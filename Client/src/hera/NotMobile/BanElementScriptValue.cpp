#include "BanElementScriptValue.h"
#include "BanStateElement.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BanElementScriptValue::BanElementScriptValue(QScriptEngine *engine, BanScriptWidget* widget, QString path):
QObject(engine),
_engine(engine),
_widget(widget),
_element(NULL)
{
	vector<BanElement*> elements  = BanFile::LoadAsScElements(path);
	for(vector<BanElement*>::iterator i = elements.begin(); i != elements.end(); ++i)
	{
		_element = *i;
		break;
	}

	_element->SetCanPaint(false);
	_element->SetCanPreview(false);
	_widget->AddElement(_element, true, false);
//	setProperty(rect.width(), "width", QScriptValue::ReadOnly);
//	setProperty(rect.height(), "height", QScriptValue::ReadOnly);
//	setProperty("draw", _engine->newFunction());
	/*
	//value.setProperty("width", rect.width(), QScriptValue::ReadOnly);
	//value.setProperty("height", rect.height(), QScriptValue::ReadOnly);
	//value.setProperty("height", _engine->newFunction());
	//value.setData(element);
	QScriptValue data = _engine->newVariant(qVariantFromValue(value));
	data.setData(data);
*/
}

BanElementScriptValue::~BanElementScriptValue()
{
}

void BanElementScriptValue::InitScriptValue(QScriptValue& value)
{
	QScriptValue getWidth = _engine->newFunction(BanElementScriptValue::getWidthFunction, this);
	value.setProperty(tr("宽"), getWidth);
	value.setProperty(tr("width"), getWidth);
	QScriptValue getHeight = _engine->newFunction(BanElementScriptValue::getHeightFunction, this);
	value.setProperty(tr("高"), getHeight);
	value.setProperty(tr("height"), getHeight);
	QScriptValue draw = _engine->newFunction(BanElementScriptValue::drawFunction, this);
	value.setProperty(tr("画"), draw);
}

QScriptValue BanElementScriptValue::ScriptValue()
{
	QScriptValue value = _engine->newQObject(this);
	InitScriptValue(value);
	return value;
}

//这里有个问题，如果draw2次呢？或者duplicate???
//先不管了，2014.10.30
//好像不可以工作，因为all是set,所以addelement只有一份，而drawing??是同一个加了2次，值覆盖了，
QScriptValue BanElementScriptValue::draw(int x, int y)
{
	BanStateElement* element = new BanStateElement(_element);
	QPoint point(x, y);
	element->UpdateSize(point);
	_widget->AddElement(element, true, false);
	element->SetLineWidth(_widget->_lineWidth);
	element->SetLineStyle(_widget->_lineStyle);
	element->SetLineColor(_widget->_lineQColor);
	_widget->DeHighlightElement();
	return QScriptValue();
}

int BanElementScriptValue::GetWidth()
{
	QRect rect = _element->GetActualRect();
	return rect.width();
}

int BanElementScriptValue::GetHeight()
{
	QRect rect = _element->GetActualRect();
	return rect.height();
}

QScriptValue BanElementScriptValue::getWidthFunction(QScriptContext* context, QScriptEngine* engine, void* param)
{
	BanElementScriptValue* value = (BanElementScriptValue*)(param);
	return value->GetWidth();
}

QScriptValue BanElementScriptValue::getHeightFunction(QScriptContext* context, QScriptEngine* engine, void* param)
{
	BanElementScriptValue* value = (BanElementScriptValue*)(param);
	return value->GetHeight();
}

QScriptValue BanElementScriptValue::drawFunction(QScriptContext* context, QScriptEngine* engine, void* param)
{
	BanElementScriptValue* value = (BanElementScriptValue*)(param);
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
