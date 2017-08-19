#pragma once

#include "BanElement.h"
#include "BanScriptWidget.h"

class LIANGSHIEXPORT BanElementScriptValue : public QObject
{
	Q_OBJECT
public:
	BanElementScriptValue(QScriptEngine *engine, BanScriptWidget* widget, QString path);
	virtual ~BanElementScriptValue();
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
	BanElement* _element;
};

