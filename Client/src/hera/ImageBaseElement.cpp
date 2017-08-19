#include "ImageBaseElement.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ImageBaseElement::ImageBaseElement()
{
}

ImageBaseElement::~ImageBaseElement()
{
}

void ImageBaseElement::SaveInitState()
{
	_initState.leftTop = _leftTop.ToQPoint();
	_initState.width = _width;
	_initState.height = _height;
	_initState.matrix = _matrix;
}

void ImageBaseElement::ToInitState()
{
	_currentState.leftTop = _leftTop.ToQPoint();
	_currentState.width = _width;
	_currentState.height = _height;
	_currentState.matrix = _matrix;

	SetState(_initState);
}

void ImageBaseElement::ToCurrentState()
{
	SetState(_currentState);
}

ImageBaseElement::State ImageBaseElement::GetState()
{
	ToInitState();
	ToCurrentState();
	return _currentState;
}

void ImageBaseElement::SetState(State state)
{
	UpdateSize(state.leftTop.ToQPoint(), state.width, state.height);
	_matrix = state.matrix;
}

void ImageBaseElement::UpdateSize(QPoint point, int width, int height)
{
	_leftTop = point;
	if(width != -1)
		_width = width;
	if(height != -1)
		_height = height;
}

bool ImageBaseElement::IsPointIn(QPoint point)
{
	return GetActualRect().contains(point);
}

QRect ImageBaseElement::GetActualRect()
{
	QRectF originRect (_leftTop._x, _leftTop._y, _width, _height);
	QMatrix matrix = _matrix.ToQMatrix();
	QRect rect = matrix.mapRect(originRect.toRect());
	return rect;
}

QPoint ImageBaseElement::GetLeftTop()
{
	return _leftTop.ToQPoint();
}

int ImageBaseElement::GetWidth()
{
	return _width;
}

int ImageBaseElement::GetHeight()
{
	return _height;
}

void ImageBaseElement::Highlight(QWidget* parent, bool high)
{
	Element::Highlight(parent, high);
	if(high)
	{
		if(_helper == NULL)
			_helper = new ImageElementHelper(this, parent);
		_helper->show();
	}
	else
	{
		if(_helper != NULL)
			_helper->hide();
	}
}

void ImageBaseElement::DrawOnQtPainter(QPainter* painter)
{
	painter->save();
	if(!IsHightlighting())
	{
		QMatrix matrix = _matrix.ToQMatrix();
		painter->setWorldMatrix(matrix, true);
		QSize size(_width, _height);
		QPixmap scaledPix = _cachePixmap.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		painter->drawPixmap(_leftTop._x, _leftTop._y, scaledPix.width(), scaledPix.height(), scaledPix);
	}
	painter->restore();
}
