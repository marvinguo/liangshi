#include "BanScriptWidget.h"
#include "Element.h"
#include "DrawElement.h"
#include "LineElement.h"
#include "RectElement.h"
#include "EllipseElement.h"
#include "ImageElement.h"
#include "TextElement.h"
#include "HtmlTextElement.h"
#include "../common/FileUtil.h"
#include "BanScriptRuntime.h"
#include "BanScriptWidget.h"
#include "BanScriptDepsDialog.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BanScriptWidget::BanScriptWidget(bool showDebugerEditor, QWidget *parent):
BanBaseWidget(parent),
_scriptRuntime(NULL),
_codeEdit(NULL),
_showDebugerEditor(showDebugerEditor)
{
	setMouseTracking(false);
	_scriptRuntime = new BanScriptRuntime(this);
	_deps = new BanScriptDeps();
	_disableHighlight = true;
	_disableDraw = true;
	_disablePaste = true;
	_disableDrop = true;
	setCursor(QCursor(Qt::ArrowCursor));
	//	setCursor(QCursor(Qt::BlankCursor));

	_codeEdit = new BanScriptEditor();
	if(_showDebugerEditor)
	{
		_codeEdit->show();
	}
}

BanScriptWidget::~BanScriptWidget()
{
	if(_codeEdit != NULL)
	{
		_codeEdit->deleteLater();
		_codeEdit = NULL;
	}
	if(_deps != NULL)
	{
		delete _deps;
		_deps = NULL;
	}
}

void BanScriptWidget::UpdateDataFromMsgPack()
{
	QString code = FileUtil::UnZipString(_zipCode);
	if(!code.isEmpty())
	{
		if(_codeEdit != NULL)
		{
			_codeEdit->setPlainText(code);
		}

		//2013.8.2现在才想到，其实并不需要script,因为数据也保存了
		//现在的事情变的有点好玩的
		if(_showDebugerEditor && false)
		{
			_scriptRuntime->RunScript(code);
		}
	}
	return BanBaseWidget::UpdateDataFromMsgPack();
}

bool BanScriptWidget::SaveDocument(QString filepath)
{
	//利用_script检测是否是程序模式，然后保存前必须把程序执行，保存就有意义了
	if(_scriptRuntime != NULL)
	{
		OnRunScriptTriggered();
	}
	return BanBaseWidget::SaveDocument(filepath);
}

void BanScriptWidget::OnScriptEditorTriggered()
{
	if(_codeEdit->isVisible())
	{
		_codeEdit->hide();
	}
	else
	{
		_codeEdit->show();
	}
}

void BanScriptWidget::OnScriptDebuggerTriggered()
{
	_dirty = true;
	QString code =_codeEdit->toPlainText();
	if(_needSaveScript)
		_zipCode = FileUtil::ZipString(code);
	_scriptRuntime->RunInDebugger(code);
}

void BanScriptWidget::OnRunScriptTriggered()
{
	_dirty = true;
	QString code =_codeEdit->toPlainText();
	if(_needSaveScript)
		_zipCode = FileUtil::ZipString(code);
	_scriptRuntime->RunScript(code);
}

void BanScriptWidget::OnShowDeps()
{
	BanScriptDepsDialog* depsDialog = BanScriptDepsDialog::Instance();
	depsDialog->ShowModal(_deps, this);
}

void BanScriptWidget::InitDeps(QString depsFolder)
{
	_deps->Init(depsFolder);
}

void BanScriptWidget::RunScript(QString code)
{
	_dirty = true;
	if(_needSaveScript)
		_zipCode = FileUtil::ZipString(code);
	_scriptRuntime->RunScript(code);
}

void BanScriptWidget::NeedSaveScript(bool need)
{
	_needSaveScript = need;
}

void BanScriptWidget::clearAll()
{
	ClearAll();
}

void BanScriptWidget::reset()
{
	_lastOrigin = QPoint(0,0);
	_lastRotate = 0;
	_lastScale = QPointF(1, 1);
	_elementMatrix.reset();
	clearAll();
	_lineWidth = 2;
	_lineStyle = Qt::SolidLine;
	_lineQColor = Qt::white;
	_lineColor = _lineQColor.rgba();
	Repaint();
}

QPointF BanScriptWidget::setOrigin(int x, int y)
{
	QPoint need = _lastOrigin;
	int xx = _lastOrigin.x();
	int yy = _lastOrigin.y();
	_elementMatrix.translate(-xx, -yy);
	_elementMatrix.translate(x, y);
	_lastOrigin.setX(x);
	_lastOrigin.setY(y);
	return need;
}

double BanScriptWidget::rotate(double angle)
{
	double need = _lastRotate;
	_elementMatrix.rotate(-_lastRotate);
	_lastRotate = angle;
	_elementMatrix.rotate(_lastRotate);
	return need;
}

QPointF BanScriptWidget::scale(double x, double y)
{
	QPointF need = _lastScale;
	double lastX = double(1)/_lastScale.x();
	double lastY = double(1)/_lastScale.y();
	_elementMatrix.scale(lastX, lastY);
	_lastScale = QPointF(x, y);
	_elementMatrix.scale(x, y);
	return need;
}

void BanScriptWidget::drawLine(int x1, int y1, int x2, int y2)
{
	LineElement* element = new LineElement();
	QPoint start(x1, y1);
	QPoint end(x2, y2);
	element->UpdatePoints(start, end);
	AddElement(element);
}

void BanScriptWidget::drawRect(int x1, int y1, int width, int height)
{
	RectElement* element = new RectElement();
	QPoint start(x1, y1);
	QPoint end(x1 + width, y1 + height);
	element->UpdatePoints(start, end);
	AddElement(element);
}

void BanScriptWidget::drawEllipse(int x1, int y1, int width, int height)
{
	EllipseElement* element = new EllipseElement();
	QPoint start(x1, y1);
	QPoint end(x1 + width, y1 + height);
	element->UpdatePoints(start, end);
	AddElement(element);
}

void BanScriptWidget::drawPath(vector<QPoint>& points)
{
	//这里有修正的，看起来是必须的
	DrawElement* element = new DrawElement();
	if(element->UpdatePoints(points))
	{
		AddElement(element);
	}
	else
	{
		delete element;
	}
}

void BanScriptWidget::drawText(int x, int y, QString& text, int size)
{
	QPoint point(x, y);
	HtmlTextElement* element = new HtmlTextElement(point, text, size);
	AddElement(element, true, false);
	//hack, need to update size
	element->UpdateText(text);
	Repaint();
}
