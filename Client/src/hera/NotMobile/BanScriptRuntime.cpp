#include "BanScriptRuntime.h"
#include "BanScriptWidget.h"
#include "TextElement.h"
#include "HtmlTextElement.h"
#include "BanElementScriptValue.h"
#include "ImageElementScriptValue.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BanScriptRuntime::BanScriptRuntime(BanScriptWidget *parent):
QObject((QObject*)parent),
_widget(parent),
_engine(new QScriptEngine(this))
{
	InitInternalObjects();

	_debugger = new QScriptEngineDebugger(_widget);
	_debugger->setAutoShowStandardWindow(true);
	_debugWindow = _debugger->standardWindow();
	_debugWindow->setWindowIcon(QIcon(":/Resources/AppIcons/collect.png"));
	_debugWindow->setWindowModality(Qt::ApplicationModal);
	_debugWindow->resize(800, 600);
}

BanScriptRuntime::~BanScriptRuntime()
{
}

void BanScriptRuntime::InitInternalObjects()
{
	//document or maybe need to change name to doc
	QScriptValue doc = _engine->newQObject(this, QScriptEngine::ScriptOwnership, QScriptEngine::ExcludeSuperClassContents);
	_engine->globalObject().setProperty(tr("doc"), doc);
	_engine->globalObject().setProperty(tr("板书"), doc);
	_engine->globalObject().setProperty(tr("黑板"), doc);

	AddChineseFunction(doc, tr("保存程序"), BanScriptRuntime::needSaveScriptFunction);
	AddChineseFunction(doc, tr("设置线宽"), BanScriptRuntime::changeLineWidthFunction);
	AddChineseFunction(doc, tr("设置线型"), BanScriptRuntime::changeLineStyleFunction);
	AddChineseFunction(doc, tr("设置线颜色"), BanScriptRuntime::changeLineColorFunction);
	AddChineseFunction(doc, tr("设置原点"), BanScriptRuntime::setOriginFunction);
	AddChineseFunction(doc, tr("旋转"), BanScriptRuntime::rotateFunction);
	AddChineseFunction(doc, tr("缩放"), BanScriptRuntime::scaleFunction);
	AddChineseFunction(doc, tr("画直线"), BanScriptRuntime::drawLineFunction);
	AddChineseFunction(doc, tr("画矩形"), BanScriptRuntime::drawRectFunction);
	AddChineseFunction(doc, tr("画椭圆"), BanScriptRuntime::drawEllipseFunction);
	AddChineseFunction(doc, tr("写字"), BanScriptRuntime::drawTextFunction);
	AddChineseFunction(doc, tr("文字大小"), BanScriptRuntime::measureTextFunction);
	AddChineseFunction(doc, tr("画曲线"), BanScriptRuntime::drawPathFunction);
	AddChineseFunction(doc, tr("画路径"), BanScriptRuntime::drawPathFunction);

	AddChineseFunction(_engine->globalObject(), tr("include"), BanScriptRuntime::includeFunction);
	AddChineseFunction(_engine->globalObject(), tr("引入"), BanScriptRuntime::includeFunction);
	AddChineseFunction(_engine->globalObject(), tr("加载程序"), BanScriptRuntime::includeFunction);
	AddChineseFunction(_engine->globalObject(), tr("debug"), BanScriptRuntime::debugFunction);
	AddChineseFunction(_engine->globalObject(), tr("调试"), BanScriptRuntime::debugFunction);
	AddChineseFunction(_engine->globalObject(), tr("loadText"), BanScriptRuntime::loadTextFunction);
	AddChineseFunction(_engine->globalObject(), tr("加载文本"), BanScriptRuntime::loadTextFunction);
	AddChineseFunction(_engine->globalObject(), tr("loadBan"), BanScriptRuntime::loadBanFunction);
	AddChineseFunction(_engine->globalObject(), tr("加载板书"), BanScriptRuntime::loadBanFunction);
	AddChineseFunction(_engine->globalObject(), tr("loadImage"), BanScriptRuntime::loadImageFunction);
	AddChineseFunction(_engine->globalObject(), tr("加载图片"), BanScriptRuntime::loadImageFunction);

	//Point function
	QScriptValue point = _engine->newFunction(BanScriptRuntime::PointFunction);
	_engine->globalObject().setProperty(tr("Point"), point);
	_engine->globalObject().setProperty(tr("点"), point);
}

QScriptValue BanScriptRuntime::RunScript(const QString &code)
{
	_widget->reset();
	_debugger->detach();
	_debugWindow->hide();
	return _engine->evaluate(code);
}

QScriptValue BanScriptRuntime::RunInDebugger(const QString &code)
{
	_widget->reset();
	_debugger->attachTo(_engine);
	_debugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
	//_debugWindow->show();
	return _engine->evaluate(code);
}

void BanScriptRuntime::AddChineseFunction(QScriptValue doc, QString name, FunctionWithArgSignature signature)
{
	QScriptValue scriptValue = _engine->newFunction(signature, this);
	doc.setProperty(name, scriptValue);
}

QScriptValue BanScriptRuntime::PointFunction(QScriptContext *context, QScriptEngine* engine)
{
	if (context->isCalledAsConstructor())
	{
		if(context->argumentCount() == 2)
		{
			QScriptValue x = context->argument(0);
			QScriptValue y = context->argument(1);
			context->thisObject().setProperty("x", x);
			context->thisObject().setProperty("y", y);
			return context->thisObject();
		}
	}
	return engine->undefinedValue();
}

QScriptValue BanScriptRuntime::needSaveScriptFunction(QScriptContext *context, QScriptEngine* engine, void* param)
{
	BanScriptRuntime* runtime = (BanScriptRuntime*)(param);
	if (context->argumentCount() == 1)
	{
		QScriptValue parameter = context->argument(0);
		if (parameter.isBool())
		{
			bool need = parameter.toBool();
			return runtime->needSaveScript(need);
		}
	}
	return QScriptValue();
}

QScriptValue BanScriptRuntime::changeLineWidthFunction(QScriptContext *context, QScriptEngine* engine, void* param)
{
	BanScriptRuntime* runtime = (BanScriptRuntime*)(param);
	if (context->argumentCount() == 1)
	{
		QScriptValue parameter = context->argument(0);
		if (parameter.isNumber())
		{
			int width = parameter.toInt32();
			return runtime->changeLineWidth(width);
		}
	}
	return QScriptValue();
}

QScriptValue BanScriptRuntime::changeLineStyleFunction(QScriptContext *context, QScriptEngine* engine, void* param)
{
	BanScriptRuntime* runtime = (BanScriptRuntime*)(param);
	if (context->argumentCount() == 1)
	{
		QScriptValue parameter = context->argument(0);
		if (parameter.isString())
		{
			QString style = parameter.toString();
			return runtime->changeLineStyle(style);
		}
	}
	return QScriptValue();
}

QScriptValue BanScriptRuntime::changeLineColorFunction(QScriptContext *context, QScriptEngine* engine, void* param)
{
	BanScriptRuntime* runtime = (BanScriptRuntime*)(param);
	if (context->argumentCount() == 1)
	{
		QScriptValue parameter = context->argument(0);
		if (parameter.isString())
		{
			QString color = parameter.toString();
			return runtime->changeLineColor(color);
		}
	}
	return QScriptValue();
}

QScriptValue BanScriptRuntime::setOriginFunction(QScriptContext *context, QScriptEngine* engine, void* param)
{
	BanScriptRuntime* runtime = (BanScriptRuntime*)(param);
	if (context->argumentCount() == 2)
	{
		QScriptValue parameter1 = context->argument(0);
		QScriptValue parameter2 = context->argument(1);
		if (parameter1.isNumber() && parameter2.isNumber())
		{
			int x = parameter1.toInt32();
			int y = parameter2.toInt32();
			return runtime->setOrigin(x, y);
		}
	}
	return QScriptValue();
}

QScriptValue BanScriptRuntime::rotateFunction(QScriptContext *context, QScriptEngine* engine, void* param)
{
	BanScriptRuntime* runtime = (BanScriptRuntime*)(param);
	if(context->argumentCount() == 1)
	{
		QScriptValue parameter1 = context->argument(0);
		if (parameter1.isNumber())
		{
			double angle = parameter1.toNumber();
			return runtime->rotate(angle);
		}
	}
	return QScriptValue();
}

QScriptValue BanScriptRuntime::scaleFunction(QScriptContext *context, QScriptEngine* engine, void* param)
{
	BanScriptRuntime* runtime = (BanScriptRuntime*)(param);
	if(context->argumentCount() == 2)
	{
		QScriptValue parameter1 = context->argument(0);
		QScriptValue parameter2 = context->argument(1);
		if (parameter1.isNumber() && parameter2.isNumber())
		{
			double x = parameter1.toNumber();
			double y = parameter2.toNumber();
			return runtime->scale(x, y);
		}
	}
	return QScriptValue();
}

QScriptValue BanScriptRuntime::drawLineFunction(QScriptContext *context, QScriptEngine* engine, void* param)
{
	BanScriptRuntime* runtime = (BanScriptRuntime*)(param);
	if(context->argumentCount() == 4)
	{
		QScriptValue parameter1 = context->argument(0);
		QScriptValue parameter2 = context->argument(1);
		QScriptValue parameter3 = context->argument(2);
		QScriptValue parameter4 = context->argument(3);

		if (parameter1.isNumber() && parameter2.isNumber() && parameter3.isNumber() && parameter4.isNumber())
		{
			int x1 = parameter1.toInt32();
			int y1 = parameter2.toInt32();
			int x2 = parameter3.toInt32();
			int y2 = parameter4.toInt32();
			return runtime->drawLine(x1, y1, x2, y2);
		}
	}
	return QScriptValue();
}

QScriptValue BanScriptRuntime::drawRectFunction(QScriptContext *context, QScriptEngine* engine, void* param)
{
	BanScriptRuntime* runtime = (BanScriptRuntime*)(param);
	if (context->argumentCount() == 4)
	{
		QScriptValue parameter1 = context->argument(0);
		QScriptValue parameter2 = context->argument(1);
		QScriptValue parameter3 = context->argument(2);
		QScriptValue parameter4 = context->argument(3);

		if (parameter1.isNumber() && parameter2.isNumber() && parameter3.isNumber() && parameter4.isNumber())
		{
			int x1 = parameter1.toInt32();
			int y1 = parameter2.toInt32();
			int width = parameter3.toInt32();
			int height = parameter4.toInt32();
			return runtime->drawRect(x1, y1, width, height);
		}
	}
	return QScriptValue();
}

QScriptValue BanScriptRuntime::drawEllipseFunction(QScriptContext *context, QScriptEngine* engine, void* param)
{
	BanScriptRuntime* runtime = (BanScriptRuntime*)(param);
	if (context->argumentCount() == 4)
	{
		QScriptValue parameter1 = context->argument(0);
		QScriptValue parameter2 = context->argument(1);
		QScriptValue parameter3 = context->argument(2);
		QScriptValue parameter4 = context->argument(3);

		if (parameter1.isNumber() && parameter2.isNumber() && parameter3.isNumber() && parameter4.isNumber())
		{
			int x1 = parameter1.toInt32();
			int y1 = parameter2.toInt32();
			int width = parameter3.toInt32();
			int height = parameter4.toInt32();
			return runtime->drawEllipse(x1, y1, width, height);
		}
	}
	return QScriptValue();
}

QScriptValue BanScriptRuntime::drawTextFunction(QScriptContext *context, QScriptEngine* engine, void* param)
{
	BanScriptRuntime* runtime = (BanScriptRuntime*)(param);
	if(context->argumentCount() == 4)
	{
		QScriptValue parameter1 = context->argument(0);
		QScriptValue parameter2 = context->argument(1);
		QScriptValue parameter3 = context->argument(2);
		QScriptValue parameter4 = context->argument(3);

		if (parameter1.isNumber() && parameter2.isNumber() && parameter3.isString() && parameter4.isNumber())
		{
			int x1 = parameter1.toInt32();
			int y1 = parameter2.toInt32();
			QString text = parameter3.toString();
			int size = parameter4.toInt32();
			return runtime->drawText(x1, y1, text, size);
		}
	}
	return QScriptValue();
}

QScriptValue BanScriptRuntime::measureTextFunction(QScriptContext *context, QScriptEngine* engine, void* param)
{
	BanScriptRuntime* runtime = (BanScriptRuntime*)(param);
	if(context->argumentCount() == 2)
	{
		QScriptValue parameter1 = context->argument(0);
		QScriptValue parameter2 = context->argument(1);

		if(parameter1.isString() && parameter2.isNumber())
		{
			QString text = parameter1.toString();
			int size = parameter2.toInt32();
			return runtime->measureText(text, size);
		}
	}
	return engine->undefinedValue();
}

QScriptValue BanScriptRuntime::drawPathFunction(QScriptContext *context, QScriptEngine* engine, void* param)
{
	BanScriptRuntime* runtime = (BanScriptRuntime*)(param);
	if (context->argumentCount() == 1)
	{
		QScriptValue parameter1 = context->argument(0);

		if (parameter1.isArray())
		{
			QVariantList points = parameter1.toVariant().toList();
			return runtime->drawPath(points);
		}
	}
	return QScriptValue();
}

QScriptValue BanScriptRuntime::loadBanFunction(QScriptContext *context, QScriptEngine* engine, void* param)
{
	BanScriptRuntime* runtime = (BanScriptRuntime*)(param);
	if (context->argumentCount() == 1)
	{
		QScriptValue parameter1 = context->argument(0);

		if (parameter1.isString())
		{
			QString filename = parameter1.toString();
			return runtime->loadBan(filename);
		}
	}
	return QScriptValue();
}

QScriptValue BanScriptRuntime::drawBanFunction(QScriptContext *context, QScriptEngine* engine, void* param)
{
	return QScriptValue();
}

QScriptValue BanScriptRuntime::loadImageFunction(QScriptContext *context, QScriptEngine* engine, void* param)
{
	BanScriptRuntime* runtime = (BanScriptRuntime*)(param);
	if (context->argumentCount() == 1)
	{
		QScriptValue parameter1 = context->argument(0);
		if (parameter1.isString())
		{
			QString path = parameter1.toString();
			return runtime->loadImage(path);
		}
	}
	return QScriptValue();
}

QScriptValue BanScriptRuntime::drawImageFunction(QScriptContext *context, QScriptEngine* engine, void* param)
{
	return QScriptValue();
}

QScriptValue BanScriptRuntime::loadTextFunction(QScriptContext* context, QScriptEngine* engine, void* param)
{
	BanScriptRuntime* runtime = (BanScriptRuntime*)(param);
	if (context->argumentCount() == 1)
	{
		QScriptValue parameter1 = context->argument(0);
		if (parameter1.isString())
		{
			QString path = parameter1.toString();
			return runtime->loadText(path);
		}
	}
	return QScriptValue();
}

QScriptValue BanScriptRuntime::needSaveScript(bool need)
{
	_widget->NeedSaveScript(need);
	return QScriptValue();
}

QScriptValue BanScriptRuntime::changeLineWidth(int width)
{
	_widget->LineWidthChanged(width);
	return QScriptValue();
}

QScriptValue BanScriptRuntime::changeLineStyle(QString style)
{
	int styleValue = Qt::SolidLine;
	if(style == tr("实线") || style == tr("solid"))
		styleValue = Qt::SolidLine;
	if(style == tr("虚线") || style == tr("dot"))
		styleValue = Qt::DotLine;
	_widget->LineStyleChanged(styleValue);
	return QScriptValue();
}

QScriptValue BanScriptRuntime::changeLineColor(QString color)
{
	//"#123456 red?
	QColor c(color);
	_widget->LineColorChanged(c);
	return QScriptValue();
}

QScriptValue BanScriptRuntime::setOrigin(int x, int y)
{
	QPointF point = _widget->setOrigin(x, y);
	QScriptValue value;
	value.setProperty("x", point.x());
	value.setProperty("y", point.y());
	return value;
}

QScriptValue BanScriptRuntime::rotate(double angle)
{
	double need = _widget->rotate(angle);
	QScriptValue value;
	value.setProperty("rotate", need);
	value.setProperty("原来值", need);
	return need;
}

QScriptValue BanScriptRuntime::scale(double x, double y)
{
	QPointF point = _widget->scale(x, y);
	QScriptValue value;
	value.setProperty("x", point.x());
	value.setProperty("y", point.y());
	return value;
}

QScriptValue BanScriptRuntime::drawLine(int x1, int y1, int x2, int y2)
{
	_widget->drawLine(x1, y1, x2, y2);
	return QScriptValue();
}

QScriptValue BanScriptRuntime::drawRect(int x1, int y1, int width, int height)
{
	_widget->drawRect(x1, y1, width, height);
	return QScriptValue();
}

QScriptValue BanScriptRuntime::drawEllipse(int x1, int y1, int width, int height)
{
	_widget->drawEllipse(x1, y1, width, height);
	return QScriptValue();
}

QScriptValue BanScriptRuntime::drawText(int x1, int y1, QString text, int size)
{
	_widget->drawText(x1, y1, text, size);
	return QScriptValue();
}

QScriptValue BanScriptRuntime::measureText(QString text, int size)
{
	HtmlTextElement element(QPoint(0, 0), text, size);
	QRect rect = element.GetActualRect();
	QScriptValue value = _engine->newObject();
	value.setProperty("width", rect.width());
	value.setProperty("height", rect.height());
	return value;
}

QScriptValue BanScriptRuntime::drawPath(QVariantList points)
{
	vector<QPoint> vectorPoints;
	for (QVariantList::const_iterator it = points.begin(); it != points.end(); ++it)
	{
		QVariant element = *it;
		if (element.canConvert<QVariantMap>())
		{
			QVariantMap map = element.toMap();
			//这里已经是一个point
			QPoint point;
			if(GetQPointFromMap(map, point))
			{
				vectorPoints.push_back(point);
			}
		}
	}
	_widget->drawPath(vectorPoints);
	return QScriptValue();
}

QScriptValue BanScriptRuntime::loadBan(QString filename)
{
	QString fullpath;
	if(!_widget->_deps->HasDepsFile(filename, fullpath))
	{
		return _engine->currentContext()->throwError(	QString("不存在依赖文件:%1").arg(filename));
	}

	BanElementScriptValue * script = new BanElementScriptValue(_engine, _widget, fullpath);
	return script->ScriptValue();
}

QScriptValue BanScriptRuntime::loadImage(QString filename)
{
	QString fullpath;
	if(!_widget->_deps->HasDepsFile(filename, fullpath))
	{
		return _engine->currentContext()->throwError(	QString("不存在依赖文件:%1").arg(filename));
	}

	ImageElementScriptValue* script = new ImageElementScriptValue(_engine, _widget,fullpath);
	return script->ScriptValue();
}

QScriptValue BanScriptRuntime::loadText(QString filename)
{
	QString fullpath;
	if(!_widget->_deps->HasDepsFile(filename, fullpath))
	{
		return _engine->currentContext()->throwError(	QString("不存在依赖文件:%1").arg(filename));
	}

	QFile readFile(fullpath);
	readFile.open(QIODevice::ReadOnly);
	QString all = readFile.readAll();
	readFile.close();
	return all;
}

bool BanScriptRuntime::GetQPointFromMap(QVariantMap& map, QPoint& point)
{
	bool hasX = false;
	bool hasY = false;
	for(QVariantMap::const_iterator iter = map.begin(); iter != map.end(); ++iter)
	{
		if(iter.key() == "x")
		{
			hasX = true;
			point.setX(iter.value().toInt());
		}
		if(iter.key() == "y")
		{
			hasY = true;
			point.setY(iter.value().toInt());
		}
	}
	return hasX&&hasY;
}

QScriptValue BanScriptRuntime::includeFunction(QScriptContext* context, QScriptEngine* engine, void* param)
{
	BanScriptRuntime* runtime = (BanScriptRuntime*)(param);
	if (context->argumentCount() == 1)
	{
		QScriptValue parameter = context->argument(0);
		if (parameter.isString())
		{
			QString filename = parameter.toString();

			QString fullpath;
			if(!runtime->_widget->_deps->HasDepsFile(filename, fullpath))
			{
				return context->throwError(	QString("不存在依赖文件:%1").arg(filename));
			}

			QFile file(fullpath);
			if(!file.open(QFile::ReadOnly))
			{
				return context->throwError(	QString("无法打开依赖文件:%1").arg(filename));
			}
			QTextStream textStream(&file);
			textStream.setAutoDetectUnicode(true);
			QString contents = textStream.readAll();
			file.close();
			context->setActivationObject(engine->globalObject());
			context->setThisObject(engine->globalObject());
			QScriptValue evaluateValue = engine->evaluate(contents, fullpath);
			return evaluateValue;
		}
	}
	return QScriptValue();
}

QScriptValue BanScriptRuntime::debugFunction(QScriptContext* context, QScriptEngine* engine, void* param)
{
	BanScriptRuntime* runtime = (BanScriptRuntime*)(param);
	QString result;
	for (int i = 0; i < context->argumentCount(); ++i)
	{
		if (i != 0)
			result.append(QLatin1String(" "));

		QString s = context->argument(i).toString();
		if (context->state() == QScriptContext::ExceptionState)
			break;
		result.append(s);
	}
	return context->engine()->toScriptValue(result);
}

