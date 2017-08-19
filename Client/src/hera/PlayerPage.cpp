#include "PlayerPage.h"
#include "ImageElement.h"
#include "TextElement.h"
#include "HtmlTextElement.h"
#include "BanElement.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

PlayerPage::PlayerPage(int pageWidth, int pageHeight, BanBaseWidget* viewer, QWidget *parent):
QWidget(parent),
_viewer(viewer),
_pageWidth(pageWidth),
_pageHeight(pageHeight)
{
	ResetElement();
}

PlayerPage::~PlayerPage()
{
	if(_viewer != NULL)
	{
		//_viewer->deleteLater();
		delete _viewer;
		_viewer = NULL;
	}
}

void PlayerPage::resizeEvent(QResizeEvent *e)
{
	//这里的backgound并没有resize，而是用设计时候的size
	_backgroudPixmap = QPixmap(_pageWidth, _pageHeight);
	update();
	QWidget::resizeEvent(e);
}

void PlayerPage::DrawBackground(QPixmap& pixmap)
{
	//pixmap.fill(QColor(BLACKBOARD_BACKGROUND_COLOR));
	//QPainter pixmapPainter(&_backgroudPixmap);
	//pixmapPainter.setRenderHint(QPainter::Antialiasing, true);
	_viewer->RenderPixmap(pixmap);
}

void PlayerPage::paintEvent(QPaintEvent *e)
{
	DrawBackground(_backgroudPixmap);

	QRect r = rect();
	QSize size(r.width(), r.height());
	QPixmap scaledPix = _backgroudPixmap.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	QPainter painter(this);
	painter.drawPixmap(0, 0, scaledPix.width(), scaledPix.height(), scaledPix);
	QWidget::paintEvent(e);
}

void PlayerPage::ResetElement()
{
	if(_viewer == NULL)
		return;

	_viewer->ResetElementToInit();
}

void PlayerPage::PlayAction(AnimationAction* action)
{
	switch(action->GetType())
	{
	case AnimationType_Show:
		PlayShowAction((ShowAction*)action);
		break;
	case AnimationType_Size:
		PlaySizeAction((SizeAction*)action);
		break;
	case AnimationType_Hide:
		PlayHideAction((HideAction*)action);
		break;
	case AnimationType_LineWidth:
		PlayLineWidthAction((LineWidthAction*)action);
		break;
	case AnimationType_LineStyle:
		PlayLineStyleAction((LineStyleAction*)action);
		break;
	case AnimationType_LineColor:
		PlayLineColorAction((LineColorAction*)action);
		break;
	case AnimationType_TopMost:
		PlayTopMostAction((TopMostAction*)action);
		break;
	case AnimationType_BottomMost:
		PlayBottomMostAction((BottomMostAction*)action);
		break;
	case AnimationType_TextChanged:
		PlayTextChangedAction((TextChangedAction*)action);
		break;
	}
}

void PlayerPage::PlayShowAction(ShowAction* action)
{
	if(_viewer == NULL)
		return;

	Element* element = _viewer->FindElementByUuid(action->GetTargetElementUuid());
	if(element != NULL)
	{
		_viewer->_drawingElements.push_back(element);
	}
}

void PlayerPage::PlaySizeAction(SizeAction* action)
{
	if(_viewer == NULL)
		return;

	Element* element = _viewer->FindElementByUuid(action->GetTargetElementUuid());
	if(element != NULL)
	{
		QPoint point(action->GetLeftTop()._x, action->GetLeftTop()._y);
		element->UpdateSize(point, action->GetWidth(), action->GetHeight());
	}
}

void PlayerPage::PlayHideAction(HideAction* action)
{
	if(_viewer == NULL)
		return;

	Element* element = _viewer->FindElementByUuid(action->GetTargetElementUuid());
	if(element != NULL)
	{
		_viewer->_drawingElements.remove(element);
	}
}

void PlayerPage::PlayLineWidthAction(LineWidthAction* action)
{
	if(_viewer == NULL)
		return;

	Element* element = _viewer->FindElementByUuid(action->GetTargetElementUuid());
	if(element != NULL)
	{
		element->SetLineWidth(action->GetWidth());
	}
}

void PlayerPage::PlayLineStyleAction(LineStyleAction* action)
{
	if(_viewer == NULL)
		return;

	Element* element = _viewer->FindElementByUuid(action->GetTargetElementUuid());
	if(element != NULL)
	{
		element->SetLineStyle(action->GetStyle());
	}
}

void PlayerPage::PlayLineColorAction(LineColorAction* action)
{
	if(_viewer == NULL)
		return;

	Element* element = _viewer->FindElementByUuid(action->GetTargetElementUuid());
	if(element != NULL)
	{
		element->SetLineColor(action->GetColor());
	}
}

void PlayerPage::PlayTopMostAction(TopMostAction* action)
{
	if(_viewer != NULL)
	{
		for(list<Element*>::iterator i = _viewer->_drawingElements.begin(); i != _viewer->_drawingElements.end(); ++i)
		{
			Element* e = *i;
			if(e->GetObjectUuid() == action->GetTargetElementUuid())
			{
				_viewer->_drawingElements.erase(i);
				_viewer->_drawingElements.push_back(e);
				break;
			}
		}
	}
}

void PlayerPage::PlayBottomMostAction(BottomMostAction* action)
{
	if(_viewer == NULL)
		return;

	if(_viewer != NULL)
	{
		for(list<Element*>::iterator i = _viewer->_drawingElements.begin(); i != _viewer->_drawingElements.end(); ++i)
		{
			Element* e = *i;
			if(e->GetObjectUuid() == action->GetTargetElementUuid())
			{
				_viewer->_drawingElements.erase(i);
				_viewer->_drawingElements.push_front(e);
				break;
			}
		}
	}
}

void PlayerPage::PlayTextChangedAction(TextChangedAction* action)
{
	if(_viewer == NULL)
		return;
	Element* element = _viewer->FindElementByUuid(action->GetTargetElementUuid());
	if(element != NULL)
	{
		TextElement* text = dynamic_cast<TextElement*>(element);
		if(text != NULL)
		{
			text->_text = action->GetText();
			text->_fontSize = action->GetFontSize();
		}

		HtmlTextElement* html = dynamic_cast<HtmlTextElement*>(element);
		if(html != NULL)
		{
			html->_text = action->GetText();
			html->_fontSize = action->GetFontSize();
		}
	}
}
