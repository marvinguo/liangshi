#include "HtmlElementPainter.h"

#include "../common/FontUtil.h"
#include "../common/Util.h"

//这性能差的，像屎一样
//#define QTMML
#define QWTMML

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

HtmlElementPainter::HtmlElementPainter(QObject* parent):
QObject(parent)
{
}

HtmlElementPainter::~HtmlElementPainter()
{
}

void HtmlElementPainter::SetHtmlText(QString& text, int size)
{
	_text = text;
	_size = size;

	if(Util::IsMathML(_text))
	{
		_mathDoc.setContent(_text);
		_mathDoc.setBaseFontPointSize(_size);
	}
	else
	{
		bool hasNoneEnglish = FontUtil::HasNoneEnglish(_text);
		QFont font = FontUtil::GetTextFontBySize(_size, hasNoneEnglish);
		_HTMLDoc.setDefaultFont(font);
		_HTMLDoc.setHtml(_text);
	}
}

QSizeF HtmlElementPainter::GetTextSize()
{

	QSizeF size;

	if(Util::IsMathML(_text))
	{
#ifdef QWTMML
		QElapsedTimer timer;
		timer.start();
		size = _mathDoc.size();
		qDebug() << "MathML get size time:" << timer.elapsed();
#endif
#ifdef QTMML
		doc.setContent(_text);
		doc.setBaseFontPointSize(_size);
		size = doc.size();
#endif
	}
	else
	{
		size = _HTMLDoc.size();
	}
	return size;
}

void HtmlElementPainter::Draw(QPainter* painter, QColor color)
{
	if(Util::IsMathML(_text))
	{
		DrawMathML(painter, color);
	}
	else
	{
		DrawHtml(painter, color);
	}
}

void HtmlElementPainter::DrawMathML(QPainter* painter, QColor color)
{
	bool hasNoneEnglish = FontUtil::HasNoneEnglish(_text);
	QFont font = FontUtil::GetTextFontBySize(_size, hasNoneEnglish);
	painter->setFont(font);
	QPen pen(color);
	painter->setPen(pen);

#ifdef QWTMML
	QElapsedTimer timer;
	timer.start();
	_mathDoc.setBackgroundColor(Qt::transparent);
	_mathDoc.setForegroundColor(color);
	_mathDoc.paint(painter, QPointF(0.0, 0.0));
	qDebug() << "MathML painter time:" << timer.elapsed();
#endif
#ifdef QTMML
	QtMmlDocument doc;
	doc.setContent(_text);
	doc.setBackgroundColor(Qt::transparent);
	doc.setForegroundColor(color);
	doc.setBaseFontPointSize(_size);
	doc.paint(painter, QPoint(0.0, 0.0));
#endif

}

void HtmlElementPainter::DrawHtml(QPainter* painter, QColor color)
{
	//bool hasNoneEnglish = FontUtil::HasNoneEnglish(_text);
	//QFont font = FontUtil::GetTextFontBySize(_size, hasNoneEnglish);
	//painter->setFont(font);
	QPen pen(color);
	painter->setPen(pen);

	QString text = "<font color=\"" + color.name() + "\">" + _text + "</font>";
	_HTMLDoc.setHtml(text);
	//QFontMetrics fm(font);
	//int lineSpace = 0;//fm.leading();
	_HTMLDoc.drawContents(painter);
}



