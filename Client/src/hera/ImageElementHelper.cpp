#include "ImageElementHelper.h"
#include "ImageElement.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ImageElementHelper::ImageElementHelper(ImageBaseElement* imageElement, QWidget *parent):
ElementLabelBorder(parent),
_imageElement(imageElement)
{
	_borderColor = Qt::red;
	QPixmap pixmap = _imageElement->GetPixmap();
	SetPixmap(pixmap);

	QPoint leftTop = _imageElement->GetLeftTop();

	Move(leftTop.x(), leftTop.y	());
	resize(_imageElement->GetWidth() + 2*HIGHTITEM_BORDER , _imageElement->GetHeight() + 2*HIGHTITEM_BORDER);

	_titleBar->GetDeleteButton()->show();
	_titleBar->GetTopMostButton()->show();
	_titleBar->GetBottomMostButton()->show();
	connect(_titleBar->GetDeleteButton(), &QPushButton::clicked, this, &ImageElementHelper::OnDeleteTriggered);
	connect(_titleBar->GetTopMostButton(), &QPushButton::clicked, this, &ImageElementHelper::OnTopMostTriggered);
	connect(_titleBar->GetBottomMostButton(), &QPushButton::clicked, this, &ImageElementHelper::OnBottomMostTriggered);
}

ImageElementHelper::~ImageElementHelper()
{
}

void ImageElementHelper::OnDeleteTriggered()
{
	Q_EMIT OnDeleteElement(_imageElement);
}

void ImageElementHelper::OnTopMostTriggered()
{
	Q_EMIT OnTopMostElement(_imageElement);
}

void ImageElementHelper::OnBottomMostTriggered()
{
	Q_EMIT OnBottomMostElement(_imageElement);
}

string ImageElementHelper::GetObjUuid()
{
	return _imageElement->GetObjectUuid();
}

void ImageElementHelper::showEvent(QShowEvent * e)
{
	ElementLabelBorder::showEvent(e);
}

void ImageElementHelper::hideEvent(QHideEvent * e)
{
	ElementLabelBorder::hideEvent(e);
}

void ImageElementHelper::SendSizeAnimation(const QSize& size)
{
	ElementLabelBorder::SendSizeAnimation(size);

	//看的懂吗？这是理解坐标的关键
	QRect rect = _label->rect();
	QPoint global = _label->mapToGlobal(_label->rect().topLeft());

	int x = parentWidget()->mapFromGlobal(global).x();
	int y = parentWidget()->mapFromGlobal(global).y();

	//_imageElement->_leftTop = SPoint(x, y);

	double width = rect.width();
	double height = rect.height();
	//_imageElement->_width = width;
	//_imageElement->_height = height;

	_imageElement->UpdateSize(QPoint(x, y), width, height);
	QMatrix matrix;
	_imageElement->SetMatrix(matrix);
}
