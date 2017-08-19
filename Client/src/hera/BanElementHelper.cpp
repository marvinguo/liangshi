#include "../common/Util.h"
#include "BanElementHelper.h"
#include "BanBaseWidget.h"
#include "BanElement.h"
#include "BanBaseWidget.h"
#include "HeraMainWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BanElementHelper::BanElementHelper(BanElement* materialElement, QWidget *parent):
ElementLabelBorder(parent),
_materialElement(materialElement)
{
	_borderColor = Qt::green;
	//这个地方不需要adjust的rect
	QPixmap pixmap = _materialElement->_viewer->ToPixmap(_materialElement->_xZoom, _materialElement->_yZoom, false, false);
	SetPixmap(pixmap);
	Move(_materialElement->_leftTop._x, _materialElement->_leftTop._y);
	
	_titleBar->GetDeleteButton()->show();
	//在录课模式，不应该让编辑
	if(_banBase->CanEditElementInside())
		_titleBar->GetEditButton()->show();
	_titleBar->GetTopMostButton()->show();
	_titleBar->GetBottomMostButton()->show();
	if(_materialElement->HasLabel())
	{
		_titleBar->GetTitleButton()->show();
	}

	connect(_titleBar->GetDeleteButton(), &QPushButton::clicked, this, &BanElementHelper::OnDeleteTriggered);
	connect(_titleBar->GetEditButton(), &QPushButton::clicked, this, &BanElementHelper::OnEditTriggered);
	connect(_titleBar->GetTitleButton(), &QPushButton::clicked, this, &BanElementHelper::OnTitleTriggered);
	connect(_titleBar->GetTopMostButton(), &QPushButton::clicked, this, &BanElementHelper::OnTopMostTriggered);
	connect(_titleBar->GetBottomMostButton(), &QPushButton::clicked, this, &BanElementHelper::OnBottomMostTriggered);
}

BanElementHelper::~BanElementHelper()
{
}

void BanElementHelper::OnDeleteTriggered()
{
	Q_EMIT OnDeleteElement(_materialElement);
}

void BanElementHelper::OnEditTriggered()
{
	QWidget* window = topLevelWidget();
	if(window != NULL)
	{
		window->hide();
		HeraMainWidget* hera = NULL;
		QVariant variant = qVariantFromValue((void*)_materialElement);
		if(_materialElement->GetScType() == ScType_Test)
			hera = HeraMainWidget::LoadFromLokiBufferInLayer(NewableDocType_TestEmbedded, _materialElement->_data, variant, _banBase->GetLayer() + 1);
		else
			hera = HeraMainWidget::LoadFromLokiBufferInLayer(NewableDocType_BanEmbedded, _materialElement->_data, variant, _banBase->GetLayer() + 1);
		if(hera != NULL)
		{
			connect(hera, &HeraMainWidget::OnGetScBuffer, _banBase, &BanBaseWidget::OnGetScBufferTriggered);
			connect(hera, SIGNAL(OnClosed()), window, SLOT(OnShowTriggered()));
			hera->ShowModal();
		}
	}
}

void BanElementHelper::OnTitleTriggered()
{
	if(_materialElement != NULL)
	{
		bool ok;
		Qt::WindowFlags flags = Qt::Dialog | Qt::WindowCloseButtonHint;
		flags = flags & (~Qt::WindowContextHelpButtonHint);
		QString text = QInputDialog::getText(parentWidget(), tr("输入标签"), tr("标签:"), QLineEdit::Normal, _materialElement->GetLabel(), &ok, flags);
		if(ok)
		{
			_materialElement->SetLabel(text);
			Q_EMIT OnDirty();
		}
	}
}

void BanElementHelper::OnTopMostTriggered()
{
	Q_EMIT OnTopMostElement(_materialElement);
}

void BanElementHelper::OnBottomMostTriggered()
{
	Q_EMIT OnBottomMostElement(_materialElement);
}

string BanElementHelper::GetObjUuid()
{
	return _materialElement->GetObjectUuid();
}

void BanElementHelper::showEvent(QShowEvent * e)
{
	ElementLabelBorder::showEvent(e);
}

void BanElementHelper::UpdateImage()
{
	QPixmap pixmap = _materialElement->_viewer->ToPixmap(_materialElement->_xZoom, _materialElement->_yZoom, false, false);
	SetPixmap(pixmap);
}

void BanElementHelper::hideEvent(QHideEvent * e)
{
	QRect rect = _label->rect();
	double width = rect.width();
	double height = rect.height();

	double xZoom = width / _materialElement->_width;
	double yZoom = height / _materialElement->_height;

	double xDiff = qAbs(_materialElement->_xZoom - xZoom);
	double yDiff = qAbs(_materialElement->_yZoom - yZoom);

	if(xDiff > 0.01 || yDiff > 0.01)
	{
		_materialElement->_xZoom = xZoom;
		_materialElement->_yZoom = yZoom;
		QPixmap pixmap = _materialElement->_viewer->ToPixmap(xZoom, yZoom, false);
		SetPixmap(pixmap);
	}

	ElementLabelBorder::hideEvent(e);
}

void BanElementHelper::SendSizeAnimation(const QSize& size)
{	
	ElementLabelBorder::SendSizeAnimation(size);

	QRect rect = _label->rect();
	QPoint global = _label->mapToGlobal(_label->rect().topLeft());

	int x = parentWidget()->mapFromGlobal(global).x();
	int y = parentWidget()->mapFromGlobal(global).y();

	_materialElement->_leftTop = SPoint(x, y);

	double width = rect.width();
	double height = rect.height();

	double xZoom = width / _materialElement->_width;
	double yZoom = height / _materialElement->_height;

	_materialElement->_zoomWidth = width;
	_materialElement->_zoomHeight = height;

//	_materialElement->_xZoom = xZoom;
//	_materialElement->_yZoom = yZoom;
}
