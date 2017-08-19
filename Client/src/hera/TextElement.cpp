#include "TextElement.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

TextElement::TextElement()
{
	Init(QPoint(0, 0), "", DEFAULT_FONT_SIZE);
}

TextElement::TextElement(QPoint leftTop, QString text, int fontSize)
{
	Init(leftTop, text, fontSize);
}

void TextElement::Init(QPoint leftTop, QString text, int fontSize)
{
	_helper = NULL;
	_leftTop = leftTop;
	_text = text.toStdString();
	_fontSize = fontSize;
//	_leftTop = SPoint(_cLeftTop._x - HIGHTITEM_BORDER, _cLeftTop._y - HIGHTITEM_BORDER);
	_fontWeight = 0;
	_width = 0;
	_height = 0;
}

TextElement::~TextElement()
{
	if(_helper != NULL)
	{
		_helper->deleteLater();
		_helper->close();
	}
}

void TextElement::SaveInitState()
{
	_initState.color = _lineQColor;
	_initState.leftTop = _leftTop.ToQPoint();
	_initState.size = _fontSize;
	_initState.text = _text;
}

void TextElement::ToInitState()
{
	_currentState.color = _lineQColor;
	_currentState.leftTop = _leftTop.ToQPoint();
	_currentState.size = _fontSize;
	_currentState.text = _text;

	SetLineColor(_initState.color);
	UpdateSize(_initState.leftTop);
	_fontSize = _initState.size;
	_text = _initState.text;
}

void TextElement::ToCurrentState()
{
	SetLineColor(_currentState.color);
	UpdateSize(_currentState.leftTop);
	_fontSize = _currentState.size;
	_text = _currentState.text;
}

void TextElement::UpdateSize(QPoint point, int width, int height)
{
	_leftTop = point;
	if(width != -1)
		_width = width;
	if(height != -1)
		_height = height;
}

void TextElement::SetLineWidth(int width)
{
	Element::SetLineWidth(width);
	//这个功能还是不太好用，以后再说吧
	//_fontSize = width;
	if(_helper != NULL)
	{
		_helper->SetFontParameters();
	}
}

void TextElement::SetLineColor(QColor color)
{
	Element::SetLineColor(color);
	if(_helper != NULL)
	{
		_helper->SetFontParameters();
	}
}

ElementType TextElement::GetType()
{
	return ElementType_Text;
}

bool TextElement::IsPointIn(QPoint point)
{
	return GetActualRect().contains(point);
}

QPoint TextElement::GetActualLeftTop()
{
	QRectF originRect = GetTextRect(false);
	QMatrix matrix = _matrix.ToQMatrix();
	QRect rect = matrix.mapRect(originRect.toRect());
	return rect.topLeft();
}

//这里以后和font那些合并, width, height遭弃，但并不能删除，兼容性问题
QRect TextElement::GetActualRect()
{
	QRectF originRect = GetTextRect();
	QMatrix matrix = _matrix.ToQMatrix();
	QRect rect = matrix.mapRect(originRect.toRect());
	return rect;
}

void TextElement::Highlight(QWidget* parent, bool high)
{
	Element::Highlight(parent, high);
	if(high)
	{
		if(_helper == NULL)
			_helper = new TextElementHelper(this, parent);
		_helper->show();
	}
	else
	{
		if(_helper != NULL)
			_helper->hide();
	}
}

void TextElement::DrawOnQtPainter(QPainter* painter)
{
	painter->save();
	if(!IsHightlighting())
	{
		QMatrix matrix = _matrix.ToQMatrix();
		painter->setWorldMatrix(matrix, true);

		QString text(_text.c_str());
		bool hasNoneEnglish = FontUtil::HasNoneEnglish(text);

		QFont font = FontUtil::GetTextFontBySize(_fontSize, hasNoneEnglish);
		painter->setFont(font);
		QPen pen(_lineQColor);
		painter->setPen(pen);

		int tabWidth = FontUtil::GetTabWidth(font);
		qDebug() << "TextElement tabWidth:" << tabWidth;
		//2014, 这里做android tab的问题
#ifdef Q_OS_ANDROID
		QString tabString(4, ' ');
//		text.replace(QString("\t"), tabString);
//		text.replace(QString("  "), QString("   "));
#endif
		QTextOption option;
		//option.setFlags(QTextOption::ShowTabsAndSpaces);
		option.setAlignment(Qt::AlignTop | Qt::AlignLeft);
		option.setTabStop(tabWidth);
		option.setWrapMode(QTextOption::WrapAnywhere);

		QRectF rect = GetTextRect();
		{
#ifdef Q_OS_ANDROID
//下面代码不工作
			double xZoom = 1;
			double yZoom = 1;
			QMatrix matrixAndroid;
			matrixAndroid.scale(xZoom, yZoom);
			qDebug() << "android text xZoom:" << xZoom << "yZoom:"<< yZoom;
			painter->setWorldMatrix(matrix, true);
#endif
			painter->drawText(rect, text, option);
#ifdef Q_OS_ANDROID
#endif
		}
	}
	painter->restore();
}

LokiBuffer TextElement::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

QRectF TextElement::GetTextRect(bool withLeading)
{
	int x = _leftTop._x;

	QString text(_text.c_str());
	bool hasNoneEnglish = FontUtil::HasNoneEnglish(text);

	QFont font = FontUtil::GetTextFontBySize(_fontSize, hasNoneEnglish);
	QFontMetrics fm(font);
	int ascent = fm.ascent();
	qDebug() << "text:" << text;
	qDebug() << "ascent:" << ascent;
	int descent = fm.descent();
	qDebug() << "descent:" << descent;
	int height = fm.height();
	qDebug() << "height:" << height;
	int leading = fm.leading();
	qDebug() << "leading:" << leading;
	int lineSpacing = fm.lineSpacing();
	qDebug() << "lineSpacing:" << lineSpacing;
	int minLeftBearing = fm.minLeftBearing();
	qDebug() << "minLeftBearing:" << minLeftBearing;
	int minRightBearing = fm.minRightBearing();
	qDebug() << "minRightBearing:" << minRightBearing;
	int maxWidth = fm.maxWidth();
	qDebug() << "maxWidth:" << maxWidth;
	int xHeight = fm.xHeight();
	qDebug() << "xHeight:" << xHeight;

#ifdef Q_OS_ANDROID
	if(hasNoneEnglish)
	{
		qDebug() << "textelement none english leading:" << leading;
		//leading = -(ascent/2);
		//leading = 0;//descent - HIGHTITEM_BORDER;
		leading = leading - HIGHTITEM_BORDER;
	}
#endif

	int textflags = Qt::AlignTop | Qt::AlignLeft;


	int tabWidth = FontUtil::GetTabWidth(font);
#ifdef Q_OS_ANDROID
	QString tabString(4, ' ');
//	text.replace(QString("\t"), tabString);
//	text.replace(QString("  "), QString("   "));
#endif

	QSize textsize = fm.size(textflags, text);
	int y = withLeading ? _leftTop._y + leading: _leftTop._y;
	QRectF rect(x, y, textsize.width(), textsize.height());
	return rect;
}

