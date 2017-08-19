#pragma once

#include "../common/Qt.h"
#include "../common/library.h"
#include "qwt_mml_document.h"
#include "qtmmlwidget.h"

class LIANGSHIEXPORT HtmlElementPainter : QObject
{
public:
	HtmlElementPainter(QObject* parent);
	~HtmlElementPainter();

	void SetHtmlText(QString& text, int size);
	QSizeF GetTextSize();
	void Draw(QPainter* painter, QColor color);

private:
	void DrawMathML(QPainter* painter, QColor color);
	void DrawHtml(QPainter* painter, QColor color);

private:
	QwtMathMLDocument _mathDoc;
	QTextDocument _HTMLDoc;

	QString _text;
	int _size;
};