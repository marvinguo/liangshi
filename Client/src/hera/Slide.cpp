#include "Slide.h"
#include "BanBaseWidget.h"
#include "ShowAction.h"
#include "SizeAction.h"
#include "HideAction.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

Slide::Slide(QSize size):
_background(NULL),
_actionIndex(-1),
_originalSize(size)
{
}

Slide::~Slide()
{
	if(_background != NULL)
	{
		delete _background;
	}

	for(auto shape : _shapes)
	{
		delete shape.second;
	}
	_shapes.clear();

	for(auto action : _shapeActions)
	{
		delete action.action;
	}
	_shapeActions.clear();
}

void Slide::SetBanBase(BanBaseWidget* banBase)
{
	_banBase = banBase;
}

void Slide::SetBackgroundPixmap(QPixmap& pixmap)
{
	_background = new ImageElement(QPoint(0, 0), pixmap);
}

bool Slide::AddToBanBase(ImageElement* element)
{
	if(_hasAddedToBanBase.find(_banBase->GetObjectUuid()) == _hasAddedToBanBase.end())
	{
		map<ImageElement*, bool> added;
		_hasAddedToBanBase.insert(pair<QString, map<ImageElement*, bool>>(_banBase->GetObjectUuid(), added));
	}
	auto& added = _hasAddedToBanBase[_banBase->GetObjectUuid()];
	if(added.find(element) == added.end())
	{
		LokiBuffer buffer = element->GetCopyableData();
		Element* basicElement = Element::CreateFromCopyableData(buffer);
		ImageElement* banElement = dynamic_cast<ImageElement*>(basicElement);
		if(banElement != NULL && _banBase != NULL)
		{
			//注意，这里不能paint
			banElement->SetCanPaint(false);
			_banBase->AddElement(banElement, false, false, true);
			added[element] = true;
			return true;
		}
	}
	else
	{
		_banBase->AddToDrawing(element->GetObjectUuid());
	}
	return false;
}

void Slide::AddShowAction(ImageElement* element)
{
	if(!AddToBanBase(element))
	{
		ShowAction showAction;
		showAction.SetTartgetBanUuid(_banBase->GetObjectUuid().toStdString());
		showAction.SetTartgetElementUuid(element->GetObjectUuid());
		Q_EMIT _banBase->OnAnimationAction(showAction, element);
	}
}

void Slide::AddSizeAction(ImageElement* element, QPoint leftTop, QSize size)
{
	AddToBanBase(element);
	SizeAction action;
	action.SetTartgetBanUuid(_banBase->GetObjectUuid().toStdString());
	action.SetTartgetElementUuid(element->GetObjectUuid());
	action.Update(leftTop, size.width(), size.height());
	_banBase->ChangeImageElementSize(element->GetObjectUuid(), leftTop, size);
	Q_EMIT _banBase->OnAnimationAction(action, NULL);
}

//所有的image都hide
void Slide::Hide()
{
	HideImageElement(_background);
	for(int i = 0; i < _actionIndex; ++i)
	{
		ShapeAction shapeAction = _shapeActions[i];
		int id = shapeAction.id;
		HideImageElement(_shapes[id]);
	}
}

void Slide::HideImageElement(ImageElement* element)
{
	AddToBanBase(element);
	_banBase->RemoveFromDrawing(element->GetObjectUuid());
	HideAction action;
	action.SetTartgetBanUuid(_banBase->GetObjectUuid().toStdString());
	action.SetTartgetElementUuid(element->GetObjectUuid());
	Q_EMIT _banBase->OnAnimationAction(action, element);
}

void Slide::UpdateBanBase()
{
	//就把当前的size?
	SetNewSize(_leftTop, _currentSize, true, true);
}

//所有的imageElement中
void Slide::SetNewSize(QPoint& leftTop, const QSize& size, bool withAction, bool withShow)
{
	_leftTop = leftTop;
	_currentSize = size;

	if(withAction)
	{
		if(withShow)
		{
			AddShowAction(_background);
		}
		AddSizeAction(_background, leftTop, size);
	}

	for(int i = 0; i < _actionIndex; ++i)
	{
		ShapeAction shapeAction = _shapeActions[i];
		int id = shapeAction.id;
		ImageElement* imageElement = _shapes[id];
		QPoint currentLeftTop;
		QSize currentSize;
		GetCurrentShapeSize(id, currentLeftTop, currentSize);
		
		//下面这个可能用处也不大，因为数据都在action里
		_banBase->ChangeImageElementSize(imageElement->GetObjectUuid(), currentLeftTop + _leftTop, currentSize);
		if(withAction)
		{
			if(withShow)
			{
				AddShowAction(imageElement);
			}
			AddSizeAction(imageElement, currentLeftTop + _leftTop, currentSize);
		}
	}
}

void Slide::GetCurrentShapeSize(int id, QPoint& leftTop, QSize& size)
{
	ImageElement* imageElement = _shapes[id];
	double xZoom = (double)_currentSize.width() / _originalSize.width();
	double yZoom = (double)_currentSize.height() / _originalSize.height();
	size.setWidth(imageElement->GetWidth() * xZoom);
	size.setHeight(imageElement->GetHeight() * yZoom);
	leftTop = imageElement->GetLeftTop();
	leftTop.setX(leftTop.x() * xZoom);
	leftTop.setY(leftTop.y() * yZoom);
}

void Slide::ToCurrentAction(QPixmap& pixmap)
{
	_actionIndex--;
	NextAction(pixmap);
}

bool Slide::NextAction(QPixmap& pixmap)
{
	if(_actionIndex == _shapeActions.size())
	{
		return false;
	}
	QPixmap background = _background->GetPixmap();
	pixmap = QPixmap(background.size());
	QPainter painter(&pixmap);
	painter.drawPixmap(0, 0, _background->GetPixmap());

	if(_actionIndex == -1)
	{
		AddShowAction(_background);
		AddSizeAction(_background, _leftTop, _currentSize);
		_actionIndex++;
	}
	else
	{
		for(int i = 0; i <= _actionIndex; ++i)
		{
			ShapeAction shapeAction = _shapeActions[i];
			int id = shapeAction.id;
			SizeAction* action = shapeAction.action;

			//检查后面还有没有这个shape的move，如果有，就continue;
			if(HasMoreMoveAction(i + 1, _actionIndex, id))
				continue;

			QPoint leftTop = action->GetLeftTop().ToQPoint();
			int width = action->GetWidth();
			int height = action->GetHeight();

			//这个pixmap不用管，因为窗口会缩放
			painter.drawPixmap(leftTop, _shapes[id]->GetPixmap());
			
			ImageElement* imageElement = _shapes[id];
			//imageElement里保存原始的leftop, size
			imageElement->UpdateSize(leftTop, width, height);

			QPoint currentLeftTop;
			QSize currentSize;
			GetCurrentShapeSize(id, currentLeftTop, currentSize);
			AddShowAction(imageElement);
			AddSizeAction(imageElement, currentLeftTop + _leftTop, currentSize);
		}
		_actionIndex++;
	}
	return true;
}

bool Slide::HasMoreMoveAction(int start, int end, int id)
{
	for(int i = start; i <= end; ++i)
	{
		ShapeAction shapeAction = _shapeActions[i];
		if(shapeAction.id == id)
		{
			return true;
		}
	}
	return false;
}

void Slide::ClearActions()
{
	_actionIndex = -1;
}

void Slide::AddShape(int id, QString filepath)
{
	QImage image;
	image.load(filepath);
	QFile::remove(filepath);
	QPixmap pixmap = QPixmap::fromImage(image);
	ImageElement* shape = new ImageElement(QPoint(0, 0), pixmap);
	_shapes.insert(pair<int, ImageElement*>(id, shape));
}

void Slide::AddAction(int id, SizeAction* action)
{
	_shapeActions.push_back(ShapeAction{id, (SizeAction*)action->CreateFromThis()});
}



