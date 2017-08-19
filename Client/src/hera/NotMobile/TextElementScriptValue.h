#pragma once

#include "TextElement.h"
#include "BanScriptWidget.h"

class LIANGSHIEXPORT TextElementScriptValue : public QObject
{
public:
	TextElementScriptValue(QScriptEngine *engine, BanScriptWidget* widget, QString text, int size);
	virtual ~TextElementScriptValue();
	void InitScriptValue(QScriptValue& value);
	QScriptValue ScriptValue();

public Q_SLOTS:
	QScriptValue draw(int x, int y);

private:
	int GetWidth();
	int GetHeight();

private:
	static QScriptValue getWidthFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue getHeightFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue drawFunction(QScriptContext* context, QScriptEngine* engine, void* param);

private:
	QScriptEngine* _engine;
	BanScriptWidget* _widget;
	TextElement* _element;
};

