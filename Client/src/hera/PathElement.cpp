#include "../common/library.h"
#include "PathElement.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

PathElement::PathElement()
{
}

PathElement::~PathElement()
{

}

void PathElement::SaveInitState()
{
	_initState.color = _lineQColor;
	_initState.width = _lineWidth;
	_initState.style = _lineStyle;
}

void PathElement::ToInitState()
{
	_currentState.color = _lineQColor;
	_currentState.width = _lineWidth;
	_currentState.style = _lineStyle;

	SetLineColor(_initState.color);
	SetLineWidth(_initState.width);
	SetLineStyle(_initState.style);
}

void PathElement::ToCurrentState()
{
	SetLineColor(_currentState.color);
	SetLineWidth(_currentState.width);
	SetLineStyle(_currentState.style);
}

bool PathElement::IsPointIn(QPoint point)
{
	return false;
	//return _path.contains(point);
}

QRect PathElement::GetActualRect()
{
	QRect originRect = _path.boundingRect().toRect();
	QMatrix matrix = _matrix.ToQMatrix();
	QRect rect = matrix.mapRect(originRect);
	return rect;
}

//pathElement要不要支持，还在考虑中
void PathElement::Highlight(QWidget* parent, bool high)
{
	Element::Highlight(parent, high);
}

void PathElement::DrawOnQtPainter(QPainter* painter)
{
	painter->save();
	{
		//我觉得顺序可以是有关系的
		QMatrix matrix = _matrix.ToQMatrix();
		painter->setWorldMatrix(matrix, true);

		QPen pen(_lineQColor);
		pen.setWidth(_lineWidth);
		pen.setStyle((Qt::PenStyle)_lineStyle);
		painter->setPen(pen);

		//if(qFuzzyCompare(zoom, 1))
		{
			painter->drawPath(_path);
		}
	}
	painter->restore();
}
