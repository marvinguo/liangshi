#pragma once

#include "../common/Qt.h"
#include "../common/library.h"

class BanScriptWidget;

class LIANGSHIEXPORT BanScriptRuntime : public QObject
{
	Q_OBJECT
//	Q_PROPERTY(int 改变线宽 WRITE changeLineWidth)
public:
	BanScriptRuntime(BanScriptWidget *parent = 0);
	virtual ~BanScriptRuntime();

	QScriptValue RunScript(const QString &code);
	QScriptValue RunInDebugger(const QString &code);

//中文函数
public:
// Q_INVOKABLE QScriptValue 改变线宽(int width);

//这些slots是程序导出的
public Q_SLOTS:
	QScriptValue needSaveScript(bool need);
	QScriptValue changeLineWidth(int width);
	QScriptValue changeLineStyle(QString style);
	QScriptValue changeLineColor(QString color);
	QScriptValue setOrigin(int x, int y);
	QScriptValue rotate(double angle);
	QScriptValue scale(double x, double y);

	QScriptValue drawLine(int x1, int y1, int x2, int y2);

	//想想下面，用宽度可能更合适
	QScriptValue drawRect(int x1, int y1, int width, int height);
	QScriptValue drawEllipse(int x1, int y1, int width, int height);

	//字
	QScriptValue drawText(int x1, int y1, QString text, int size);
	QScriptValue measureText(QString text, int size);

	//path
	QScriptValue drawPath(QVariantList points);

	//ban
	QScriptValue loadBan(QString filename);
	//image
	QScriptValue loadImage(QString filename);
	//image
	QScriptValue loadText(QString filename);

private:
	static QScriptValue needSaveScriptFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue changeLineWidthFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue changeLineStyleFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue changeLineColorFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue setOriginFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue rotateFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue scaleFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue drawLineFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue drawRectFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue drawEllipseFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue drawTextFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue measureTextFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue drawPathFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue loadBanFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue drawBanFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue loadImageFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue drawImageFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue loadTextFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue includeFunction(QScriptContext* context, QScriptEngine* engine, void* param);
	static QScriptValue debugFunction(QScriptContext* context, QScriptEngine* engine, void* param);

private:
	static QScriptValue PointFunction(QScriptContext* context, QScriptEngine* engine);
	void InitInternalObjects();
	bool GetQPointFromMap(QVariantMap& map, QPoint& point);
	typedef QScriptValue (*FunctionWithArgSignature)(QScriptContext* , QScriptEngine *, void *);
	void AddChineseFunction(QScriptValue doc, QString name, FunctionWithArgSignature signature);

private:
	QScriptEngine*_engine;
	BanScriptWidget*_widget;
	QScriptEngineDebugger*_debugger;
	QMainWindow *_debugWindow;
};

