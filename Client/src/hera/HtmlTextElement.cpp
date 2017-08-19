#include "HtmlTextElement.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

HtmlTextElement::HtmlTextElement()
{
	Init(QPoint(0, 0), "", DEFAULT_FONT_SIZE);
}

HtmlTextElement::HtmlTextElement(QPoint leftTop, QString text, int fontSize)
{
	Init(leftTop, text, fontSize);
}

void HtmlTextElement::Init(QPoint leftTop, QString text, int fontSize)
{
	_painter = new HtmlElementPainter(NULL);
	_helper = NULL;
	_leftTop = leftTop;
	_text = text.toStdString();
	_fontSize = fontSize;
	//	_leftTop = SPoint(_cLeftTop._x - HIGHTITEM_BORDER, _cLeftTop._y - HIGHTITEM_BORDER);
	_fontWeight = 0;
	_width = 0;
	_height = 0;
}

HtmlTextElement::~HtmlTextElement()
{
	if(_helper != NULL)
	{
		_helper->deleteLater();
		_helper->close();
	}

	if(_painter != NULL)
	{
		delete _painter;
		_painter = NULL;
	}
}

void HtmlTextElement::SaveInitState()
{
	_initState.color = _lineQColor;
	_initState.leftTop = _leftTop.ToQPoint();
	_initState.size = _fontSize;
	_initState.text = QString(_text.c_str());
}

void HtmlTextElement::ToInitState()
{
	_currentState.color = _lineQColor;
	_currentState.leftTop = _leftTop.ToQPoint();
	_currentState.size = _fontSize;
	_currentState.text = QString(_text.c_str());

	SetLineColor(_initState.color);
	UpdateSize(_initState.leftTop);
	UpdateFontSize(_initState.size);
	UpdateText(_initState.text);
}

void HtmlTextElement::ToCurrentState()
{
	SetLineColor(_currentState.color);
	UpdateSize(_currentState.leftTop);
	UpdateFontSize(_currentState.size);
	UpdateText(_currentState.text);
}

void HtmlTextElement::UpdateSize(QPoint point, int width, int height)
{
	_leftTop = point;
	if(width != -1)
		_width = width;
	if(height != -1)
		_height = height;
}

void HtmlTextElement::UpdateText(QString& text)
{
	_text = text.toStdString();
	_painter->SetHtmlText(text, _fontSize);
	QSizeF size = _painter->GetTextSize();
	_width = size.width();
	_height = size.height();
}

void HtmlTextElement::UpdateFontSize(int size)
{
	_fontSize = size;
	QString text(_text.c_str());
	_painter->SetHtmlText(text, size);
}

void HtmlTextElement::SetLineWidth(int width)
{
	Element::SetLineWidth(width);
	//_fontSize = width;
	if(_helper != NULL)
	{
	}
}

void HtmlTextElement::SetLineColor(QColor color)
{
	Element::SetLineColor(color);
	if(_helper != NULL)
	{
	}
}

ElementType HtmlTextElement::GetType()
{
	return ElementType_HtmlText;
}

bool HtmlTextElement::IsPointIn(QPoint point)
{
	return GetActualRect().contains(point);
}

QPoint HtmlTextElement::GetActualLeftTop()
{
	QRect originRect = GetActualRect();
	QMatrix matrix = _matrix.ToQMatrix();
	QRect rect = matrix.mapRect(originRect);
	return rect.topLeft();
}

//这里以后和font那些合并, width, height遭弃，但并不能删除，兼容性问题
QRect HtmlTextElement::GetActualRect()
{
	QRectF originRect (_leftTop._x, _leftTop._y, _width, _height);
	QMatrix matrix = _matrix.ToQMatrix();
	QRect rect = matrix.mapRect(originRect.toRect());
	return rect;
}

void HtmlTextElement::Highlight(QWidget* parent, bool high)
{
	Element::Highlight(parent, high);
	if(high)
	{
		if(_helper == NULL)
		_helper = new HtmlTextElementHelper(this, parent);
		_helper->show();
	}
	else
	{
		if(_helper != NULL)
			_helper->hide();
	}
}

//这里用html viewer吧，我想这样
void HtmlTextElement::DrawOnQtPainter(QPainter* painter)
{
	painter->save();
	if(!IsHightlighting())
	{
		QString text(_text.c_str());
		bool hasNoneEnglish = FontUtil::HasNoneEnglish(text);

		QMatrix matrix = _matrix.ToQMatrix();
		painter->setWorldMatrix(matrix, true);
		QFont font = FontUtil::GetTextFontBySize(_fontSize, hasNoneEnglish);
		painter->setFont(font);
		QPen pen(_lineQColor);
		painter->setPen(pen);

		painter->translate(_leftTop._x, _leftTop._y);

		_painter->SetHtmlText(text, _fontSize);
		_painter->Draw(painter, _lineQColor);
	}
	painter->restore();
}

LokiBuffer HtmlTextElement::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}



