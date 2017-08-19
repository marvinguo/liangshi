#include "BanElement.h"
#include "BanFile.h"
#include "../common/Util.h"
#include "BanBaseWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BanElement::BanElement():
Element(),
_viewer(NULL)
{
	Init(QPoint(0, 0), NULL);
}

BanElement::BanElement(BanBaseWidget* viwer):
Element(),
_viewer(NULL)
{
	Init(QCursor::pos(), viwer);
}

BanElement::~BanElement()
{
	if(_viewer != NULL)
	{
		//_viewer->deleteLater();
		delete _viewer;
		_viewer = NULL;
	}

	if(_helper != NULL)
	{
		_helper->deleteLater();
		_helper = NULL;
	}
}

void BanElement::SaveInitState()
{
	_initState.leftTop = _leftTop.ToQPoint();
	_initState.zoomWidth = _zoomWidth;
	_initState.zoomHeight = _zoomHeight;
	_initState.matrix = _matrix;
}

void BanElement::ToInitState()
{
	_currentState.leftTop = _leftTop.ToQPoint();
	_currentState.zoomWidth = _zoomWidth;
	_currentState.zoomHeight = _zoomHeight;
	_currentState.matrix = _matrix;

	SetState(_initState);
}

void BanElement::ToCurrentState()
{
	SetState(_currentState);
}

BanElement::State BanElement::GetState()
{
	ToInitState();
	ToCurrentState();
	return _currentState;
}

void BanElement::SetState(State state)
{
	UpdateSize(state.leftTop.ToQPoint(), state.zoomWidth, state.zoomHeight);
	_matrix = state.matrix;
}

void BanElement::UpdateFromBuffer(LokiBuffer& buffer)
{
	this->_data = buffer;
	_xZoom = 1;
	_yZoom = 1;
	this->UpdateDataFromMsgPack();
	if(_helper != NULL)
	{
		_helper->deleteLater();
		_helper = NULL;
	}
}

bool BanElement::IsEmptyInside()
{
	if(_viewer != NULL)
	{
		return _viewer->_drawingElements.empty();
	}
	return true;
}

//这里有问题，需有检查以前的代码
void BanElement::UpdateSize(QPoint point, int width, int height)
{
	_leftTop = point;
	if(width != -1)
		_zoomWidth = width;
	if(height != -1)
		_zoomHeight = height;
	_xZoom = (double)(_zoomWidth) / _width;
	_yZoom = (double)(_zoomHeight) / _height;
}

void BanElement::Init(QPoint leftTop, BanBaseWidget* viwer)
{
	_helper = NULL;
	_leftTop = leftTop;
	_xZoom = 1;
	_yZoom = 1;


	//这个viewer是不是在内存泄漏？
	if(viwer != NULL)
	{
		_viewer = viwer;
		InitViewer();
	}
}

void BanElement::InitViewer()
{
	//load from file
	QRect rect = _viewer->GetUnitedRect();
	_leftTopInSc = rect.topLeft();
	SetWidthHeight(rect.width(), rect.height());
	_zoomWidth = _width * _xZoom;
	_zoomHeight = _height * _yZoom;
	_data = _viewer->GetMsgPackData();
	_viewer->hide();
}

void BanElement::SetWidthHeight(int width, int height)
{
	_width = width <= 0 ? 1 : width;
	_height = height <= 0 ? 1 : height;
}

ScType BanElement::GetScType()
{
	return _viewer->GetType();
}

//当前这一层的东西改掉
//2014.11.27, 这个功能不要了，不然在录课里有问题
void BanElement::SetLineWidth(int width)
{
}

void BanElement::SetLineStyle(int style)
{
}

void BanElement::SetLineColor(QColor color)
{
}

ElementType BanElement::GetType()
{
	return ElementType_Ban;
}

bool BanElement::IsPointIn(QPoint point)
{
	return GetActualRect().contains(point);
}

QRect BanElement::GetActualRect()
{
	QRectF originRect(_leftTop._x, _leftTop._y, _zoomWidth, _zoomHeight);
	QMatrix matrix = _matrix.ToQMatrix();
	QRect rect = matrix.mapRect(originRect.toRect());
	return rect;
}

void BanElement::Highlight(QWidget* parent, bool high)
{
	Element::Highlight(parent, high);
	if(high)
	{
		if(_helper == NULL)
			_helper = new BanElementHelper(this, parent);
		_helper->show();
	}
	else
	{
		if(_helper != NULL)
			_helper->hide();
	}
}

void BanElement::DrawOnQtPainter(QPainter* painter)
{
	painter->save();
	if(!IsHightlighting())
	{
		//Material不能够用ActualRect来算，而是用0, 0来算
		//相当于translate，需要找原点和zoom
		QMatrix matrix = _matrix.ToQMatrix();
		if(matrix.isIdentity())
		{
			double inX = _leftTopInSc._x * _xZoom;
			double inY = _leftTopInSc._y * _yZoom;
			int x = _leftTop._x - floor(inX);
			int y = _leftTop._y - floor(inY);
			painter->translate(x, y);
			painter->scale(_xZoom, _yZoom);
		}
		else
		{
			painter->setWorldMatrix(matrix, true);
			QRect rect1(_leftTopInSc._x, _leftTopInSc._y, _width, _height);
			painter->translate(-rect1.topLeft());
			painter->translate(_leftTop._x, _leftTop._y);
		}
		_viewer->DrawOnQtPainter(painter);
	}
	painter->restore();
}

LokiBuffer BanElement::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

void BanElement::UpdateDataFromMsgPack()
{
	Element::UpdateDataFromMsgPack();
	if(_viewer == NULL)
	{
		_viewer = new BanBaseWidget(true);
	}
	_viewer->	SetMsgPackData(_data);
	InitViewer();
}