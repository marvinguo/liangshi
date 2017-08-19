#include "HtmlTextElementHelper.h"
#include "HtmlTextElement.h"
#include "HtmlElementEditor.h"
#include "Element.h"
#include "SizeAction.h"
#include "TextChangedAction.h"
#include "BanBaseWidget.h"
#include "NotifyWindow.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

HtmlTextElementHelper::HtmlTextElementHelper(HtmlTextElement* HtmlTextElement, QWidget *parent):
ElementLabelBorder(parent, false),
_HtmlTextElement(HtmlTextElement)
{
	_borderColor = Qt::white;
	BanBaseWidget* base = dynamic_cast<BanBaseWidget*>(parent);
	if(base != NULL)
	{
		_banBase = base;
		connect(this, &HtmlTextElementHelper::OnAnimationAction, _banBase, &BanBaseWidget::OnAnimationAction);
		connect(this, &HtmlTextElementHelper::OnDeleteElement, _banBase, &BanBaseWidget::RemoveDrawingElementTriggered);
		connect(this, &HtmlTextElementHelper::OnTopMostElement, _banBase, &BanBaseWidget::OnTopMostElementTriggered);
		connect(this, &HtmlTextElementHelper::OnBottomMostElement, _banBase, &BanBaseWidget::OnBottomMostElementTriggered);
		connect(this, SIGNAL(OnDirty()), _banBase, SLOT(SetModifiedTriggered()));
		//this->installEventFilter(_scBase);
	}

	_titleBar->GetDeleteButton()->show();
	_titleBar->GetEditButton()->show();
	_titleBar->GetTopMostButton()->show();
	_titleBar->GetBottomMostButton()->show();
	connect(_titleBar->GetDeleteButton(), &QPushButton::clicked, this, &HtmlTextElementHelper::OnDeleteTriggered);
	connect(_titleBar->GetEditButton(), &QPushButton::clicked, this, &HtmlTextElementHelper::OnEditTriggered);
	connect(_titleBar->GetTopMostButton(), &QPushButton::clicked, this, &HtmlTextElementHelper::OnTopMostTriggered);
	connect(_titleBar->GetBottomMostButton(), &QPushButton::clicked, this, &HtmlTextElementHelper::OnBottomMostTriggered);

	Move(_HtmlTextElement->_leftTop._x, _HtmlTextElement->_leftTop._y);
	QString text(_HtmlTextElement->_text.c_str());
	_HtmlTextElement->UpdateText(text);
	UpdatePixmap();
}

HtmlTextElementHelper::~HtmlTextElementHelper()
{
}

void HtmlTextElementHelper::OnDeleteTriggered()
{
	_HtmlTextElement->SetCanDelete(false);
	Q_EMIT OnDeleteElement(_HtmlTextElement);
}

void HtmlTextElementHelper::OnEditTriggered()
{
	HtmlElementEditor* editor = HtmlElementEditor::Instance();
	editor->ShowModal(_HtmlTextElement);
	UpdatePixmap();
}

void HtmlTextElementHelper::UpdatePixmap()
{
	QSize size = _HtmlTextElement->_painter->GetTextSize().toSize();

	QPixmap pixmap(size);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	_HtmlTextElement->_painter->Draw(&painter, _HtmlTextElement->_lineQColor);
	SetPixmap(pixmap, true);

	TextChangedAction action;
	action.SetTartgetBanUuid(_banBase->GetObjectUuid().toStdString());
	action.SetTartgetElementUuid(_HtmlTextElement->GetObjectUuid());
	action.Update(QString(_HtmlTextElement->_text.c_str()), _HtmlTextElement->_fontSize);
	Q_EMIT OnAnimationAction(action, _HtmlTextElement);
}

void HtmlTextElementHelper::OnTopMostTriggered()
{
	Q_EMIT OnTopMostElement(_HtmlTextElement);
}

void HtmlTextElementHelper::OnBottomMostTriggered()
{
	Q_EMIT OnBottomMostElement(_HtmlTextElement);
}

void HtmlTextElementHelper::wheelEvent(QWheelEvent *e)
{
	int numDegrees = e->delta() / 8;
	int numSteps = numDegrees / 15;

	int fontIncrease = 1;

	if(numSteps > 0)
	{
		_HtmlTextElement->_fontSize = _HtmlTextElement->_fontSize + fontIncrease;
	}
	else if(numSteps < 0)
	{
		if(_HtmlTextElement->_fontSize >= 2)
		{
			_HtmlTextElement->_fontSize = _HtmlTextElement->_fontSize - fontIncrease;
		}
	}
	e->accept();

	QString msg = tr("字体大小:") + QString::number(_HtmlTextElement->_fontSize);
	NotifyWindow* notify = NotifyWindow::Instance();
	notify->ShowNotify(msg);

	_HtmlTextElement->UpdateFontSize(_HtmlTextElement->_fontSize);
	
	//UpdatePixmap慢，但是会保证大小一样
	UpdatePixmap();
	//按图片，是用图片的放大算法，不是字的大小
	//ElementLabelBorder::wheelEvent(e);
}

void HtmlTextElementHelper::showEvent(QShowEvent * e)
{
	UpdatePixmap();
	ElementLabelBorder::showEvent(e);
}

void HtmlTextElementHelper::hideEvent(QHideEvent *e)
{
	ElementLabelBorder::hideEvent(e);

	if(_HtmlTextElement->_text.size() == 0 && _HtmlTextElement->CanDelete())
	{
		OnDeleteTriggered();
	}
}

string HtmlTextElementHelper::GetObjUuid()
{
	return _HtmlTextElement->GetObjectUuid();
}

void HtmlTextElementHelper::SendSizeAnimation(const QSize& size)
{
	ElementLabelBorder::SendSizeAnimation(size);

	//看的懂吗？这是理解坐标的关键
	QRect rect = _label->rect();
	QPoint global = _label->mapToGlobal(_label->rect().topLeft());

	int x = parentWidget()->mapFromGlobal(global).x();
	int y = parentWidget()->mapFromGlobal(global).y();

	_HtmlTextElement->_leftTop = SPoint(x, y);

	double width = rect.width();
	double height = rect.height();
	_HtmlTextElement->_width = width;
	_HtmlTextElement->_height = height;
	QMatrix matrix;
	_HtmlTextElement->SetMatrix(matrix);
}
